/* Copyright (c) 2026 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "databasearchive.h"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QSet>
#include <QStringList>
#include <QVector>
#include <algorithm>
#include <cstring>

// The existing XArchive/Algos zlib codec, without XArchive or engine objects.
#include "zlib.h"

namespace {
const quint64 MAX_ARCHIVE = 256ULL * 1024 * 1024;
const quint64 MAX_MEMBER = 64ULL * 1024 * 1024;
const quint64 MAX_EXTRACTED = 1024ULL * 1024 * 1024;
const quint64 MAX_MEMBERS = 200000;
const int BLOCK_SIZE = 64 * 1024;

quint16 u16(const QByteArray &data, int offset)
{
    const unsigned char *p = reinterpret_cast<const unsigned char *>(data.constData() + offset);
    return quint16(p[0]) | (quint16(p[1]) << 8);
}

quint32 u32(const QByteArray &data, int offset)
{
    return quint32(u16(data, offset)) | (quint32(u16(data, offset + 2)) << 16);
}

quint64 u64(const QByteArray &data, int offset)
{
    return quint64(u32(data, offset)) | (quint64(u32(data, offset + 4)) << 32);
}

struct Member {
    QString name;
    quint64 offset = 0;
    quint64 compressed = 0;
    quint64 size = 0;
    quint32 crc = 0;
    quint16 flags = 0;
    quint16 method = 0;
    bool directory = false;
};

struct Extra {
    QByteArray zip64;
    bool unsafeType = false;
};

struct Range {
    quint64 begin;
    quint64 end;
};

// A ZIP path without the UTF-8 flag uses the original IBM code page.
QString decodeName(const QByteArray &name, bool utf8)
{
    if (utf8) {
        QString result = QString::fromUtf8(name);
        return result.toUtf8() == name ? result : QString();
    }
    static const ushort cp437[] = {
        0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
        0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
        0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
        0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
        0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
        0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
        0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
        0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
        0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
        0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
        0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
        0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
        0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4,
        0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
        0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248,
        0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0
    };
    QString result;
    result.reserve(name.size());
    for (int i = 0; i < name.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(name.at(i));
        result += QChar(ch < 128 ? ch : cp437[ch - 128]);
    }
    return result;
}

bool signatureName(const QString &path)
{
    // Match CDIE db_load: readable files in these directories are counted,
    // without parsing or executing their JavaScript. Nested arbitrary folders
    // and metadata are not signature directories.
    static const QSet<QString> directories = {
        QString(), "Binary", "COM", "Archive", "ZIP", "JAR", "APK", "IPA", "NPM", "MACHOFAT", "DEX", "MSDOS", "LE", "LX", "NE", "PE",
        "PE/DOTNET", "ELF", "MACH", "DOS16M", "DOS4G", "Amiga", "AtariST", "JavaClass", "PYC", "PDF", "CFBF", "Image", "JPEG", "PNG", "RAR", "ISO9660"
    };
    QString relative = path.mid(3);  // db/
    int slash = relative.lastIndexOf('/');
    if (!directories.contains(slash < 0 ? QString() : relative.left(slash))) return false;
    QString name = relative.mid(slash + 1);
    int dot = name.lastIndexOf('.');
    return dot <= 0 || dot == name.size() - 1 || name.mid(dot + 1).compare("sg", Qt::CaseInsensitive) == 0;
}

class Extractor {
public:
    Extractor(const QString &archive, const QString &destination, QString *error, const std::function<bool()> &cancelled)
        : m_file(archive), m_destination(destination), m_error(error), m_cancelled(cancelled)
    {
    }

    ~Extractor()
    {
        if (m_createdDatabase && !m_success) QDir(m_destination + "/db").removeRecursively();
    }

    DatabaseArchive::Result extract(int *signatures)
    {
        if (stopped()) return m_result;
        if (!m_file.open(QIODevice::ReadOnly)) return failResult("Cannot open the database archive.");
        qint64 size = m_file.size();
        if (size < 22 || quint64(size) > MAX_ARCHIVE) return failResult("Database archive is empty, truncated, or exceeds 256 MiB.");
        m_size = quint64(size);
        if (!readDirectory()) return m_result;
        if (m_members.isEmpty()) return failResult("The archive does not contain a db directory.");
        QFileInfo destination(m_destination);
        if (destination.isSymLink() || (destination.exists() && !destination.isDir())) return failResult("Unsafe database staging directory.");
        if (!destination.exists() && !QDir().mkpath(m_destination)) return failResult("Cannot create the database staging directory.");
        QDir root(m_destination);
        if (!root.entryList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot).isEmpty())
            return failResult("Database staging directory is not empty.");
        m_destination = root.canonicalPath();
        if (m_destination.isEmpty() || !QDir(m_destination).mkdir("db")) return failResult("Cannot create the staged database.");
        m_createdDatabase = true;
        QFile::setPermissions(m_destination + "/db", QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
        int count = 0;
        for (const Member &member : m_members) {
            if (stopped()) return m_result;
            QString path = m_destination + '/' + member.name;
            if (!makeDirectories(member.directory ? member.name : member.name.left(member.name.lastIndexOf('/')))) return m_result;
            QFile output(path);
            if (!member.directory) {
                QFileInfo info(path);
                if (info.exists() || info.isSymLink()) return failResult("Database archive would overwrite a staged file.");
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
                if (!output.open(QIODevice::WriteOnly | QIODevice::NewOnly))
#else
                if (!output.open(QIODevice::WriteOnly))
#endif
                    return failResult("Cannot create a staged database file.");
            }
            if (!unpack(member, member.directory ? nullptr : &output)) return m_result;
            if (!member.directory) {
                if (!output.flush()) return failResult("Cannot finish writing a staged database file.");
                output.close();
                if (signatureName(member.name)) ++count;
            }
        }
        if (stopped()) return m_result;
        if (!count) return failResult("The staged database contains no usable signatures.");
        if (signatures) *signatures = count;
        m_success = true;
        return DatabaseArchive::Success;
    }

private:
    bool fail(const QString &message)
    {
        if (m_error) *m_error = message;
        m_result = DatabaseArchive::Invalid;
        return false;
    }

    DatabaseArchive::Result failResult(const QString &message)
    {
        fail(message);
        return m_result;
    }

    bool stopped()
    {
        if (m_cancelled && m_cancelled()) {
            if (m_error) *m_error = "Cancelled";
            m_result = DatabaseArchive::Cancelled;
            return true;
        }
        return false;
    }

    bool read(quint64 offset, int length, QByteArray *data)
    {
        if (offset > m_size || length < 0 || quint64(length) > m_size - offset || !m_file.seek(qint64(offset)))
            return fail("Truncated or invalid ZIP record.");
        *data = m_file.read(length);
        return data->size() == length || fail("Cannot read a complete ZIP record.");
    }

    bool parseExtra(const QByteArray &data, Extra *extra)
    {
        bool hasZip64 = false;
        for (int position = 0; position < data.size();) {
            if (data.size() - position < 4) return fail("Truncated ZIP extra field.");
            quint16 tag = u16(data, position);
            int length = u16(data, position + 2);
            position += 4;
            if (length > data.size() - position) return fail("Truncated ZIP extra field.");
            if (tag == 1) {
                if (hasZip64) return fail("Duplicate ZIP64 extra field.");
                hasZip64 = true;
                extra->zip64 = data.mid(position, length);
            } else if (tag == 0x000d) {
                // PKWARE Unix extra data after the timestamps/UID/GID denotes
                // a hard link, symbolic link or device node.
                if (length != 12) extra->unsafeType = true;
            } else if (tag == 0x756e) {
                // ASi Unix stores mode plus an optional link target.
                if (length != 14) extra->unsafeType = true;
                else {
                    quint16 type = u16(data, position + 4) & 0170000;
                    if (type != 0100000 && type != 0040000) extra->unsafeType = true;
                }
            }
            position += length;
        }
        return true;
    }

    bool take64(const QByteArray &extra, int *position, quint64 *value)
    {
        if (*position > extra.size() - 8) return fail("Missing ZIP64 size or offset.");
        *value = u64(extra, *position);
        *position += 8;
        return true;
    }

    bool directoryBounds(quint64 *offset, quint64 *length, quint64 *count)
    {
        QByteArray tail;
        int tailSize = int(qMin(m_size, quint64(65535 + 22)));
        if (!read(m_size - tailSize, tailSize, &tail)) return false;
        int end = -1;
        for (int i = tail.size() - 22; i >= 0; --i) {
            if (u32(tail, i) == 0x06054b50 && i + 22 + u16(tail, i + 20) == tail.size()) {
                end = i;
                break;
            }
        }
        if (end < 0) return fail("ZIP central directory is missing.");
        quint64 endPosition = m_size - tailSize + end;
        if (u16(tail, end + 4) || u16(tail, end + 6) || u16(tail, end + 8) != u16(tail, end + 10))
            return fail("Split ZIP archives are not supported.");
        *count = u16(tail, end + 10);
        *length = u32(tail, end + 12);
        *offset = u32(tail, end + 16);
        bool needs64 = *count == 0xffff || *length == 0xffffffff || *offset == 0xffffffff;
        QByteArray locator;
        if (endPosition >= 20 && !read(endPosition - 20, 20, &locator)) return false;
        if (locator.size() == 20 && u32(locator, 0) == 0x07064b50) {
            if (u32(locator, 4) || u32(locator, 16) != 1) return fail("Split ZIP64 archives are not supported.");
            quint64 position = u64(locator, 8);
            QByteArray record;
            if (!read(position, 56, &record)) return false;
            quint64 recordSize = u64(record, 4);
            if (u32(record, 0) != 0x06064b50 || recordSize < 44 || position > endPosition - 20 ||
                endPosition - 20 - position < 12 || recordSize != endPosition - 20 - position - 12 ||
                u32(record, 16) || u32(record, 20) || u64(record, 24) != u64(record, 32))
                return fail("Invalid ZIP64 central directory.");
            quint64 count64 = u64(record, 32), length64 = u64(record, 40), offset64 = u64(record, 48);
            if ((*count != 0xffff && *count != count64) || (*length != 0xffffffff && *length != length64) ||
                (*offset != 0xffffffff && *offset != offset64)) return fail("Inconsistent ZIP64 central directory.");
            *count = count64;
            *length = length64;
            *offset = offset64;
            endPosition = position;
        } else if (needs64) {
            return fail("ZIP64 central directory is missing.");
        }
        if (*count > MAX_MEMBERS || *offset > endPosition || *length != endPosition - *offset)
            return fail("ZIP central directory has invalid bounds or too many members.");
        return true;
    }

    bool safeName(Member *member)
    {
        if (member->name.endsWith('/')) member->name.chop(1);
        if (member->name.size() > 4096) return fail("Database member path is too long.");
        const QStringList parts = member->name.split('/');
        if (parts.isEmpty() || parts.first() != "db" || (parts.size() == 1 && !member->directory))
            return fail("Invalid database member path.");
        QString path;
        for (int index = 0; index < parts.size(); ++index) {
            const QString &part = parts.at(index);
            if (part.isEmpty() || part == "." || part == ".." || part.endsWith('.') || part.endsWith(' ') || part.toUtf8().size() > 255)
                return fail("Unsafe database member path.");
            for (QChar ch : part) {
                if (ch.unicode() < 32 || ch.unicode() == 127 || QString(":<>\"|?*").contains(ch))
                    return fail("Unsafe database member path.");
            }
            QString device = part.section('.', 0, 0).toUpper();
            if (device == "CON" || device == "PRN" || device == "AUX" || device == "NUL" || device == "CONIN$" || device == "CONOUT$" ||
                ((device.startsWith("COM") || device.startsWith("LPT")) && device.size() == 4 && QString("123456789\u00b9\u00b2\u00b3").contains(device.at(3))))
                return fail("Database archive contains a device name.");
            if (!path.isEmpty()) path += '/';
            path += part;
            QString key = path.normalized(QString::NormalizationForm_C).toCaseFolded();
            bool directory = index < parts.size() - 1 || member->directory;
            if (m_paths.contains(key)) {
                if (m_paths.value(key) != path || !directory || !m_directories.contains(key))
                    return fail("Database archive has duplicate or conflicting member paths.");
                if (index == parts.size() - 1 && m_explicit.contains(key)) return fail("Database archive has duplicate directory entries.");
            } else {
                m_paths.insert(key, path);
                if (directory) m_directories.insert(key);
            }
            if (index == parts.size() - 1) m_explicit.insert(key);
        }
        return true;
    }

    bool localRecord(Member *member, const QByteArray &rawName, bool zip64, quint64 limit, quint64 *end, bool *unsafeExtra)
    {
        QByteArray local;
        quint64 start = member->offset;
        if (start > limit || limit - start < 30 || !read(start, 30, &local)) return fail("Invalid ZIP local header offset.");
        if (u32(local, 0) != 0x04034b50 || u16(local, 6) != member->flags || u16(local, 8) != member->method || u16(local, 26) != rawName.size())
            return fail("ZIP local and central headers disagree.");
        int nameSize = u16(local, 26), extraSize = u16(local, 28);
        quint64 headerSize = quint64(30 + nameSize + extraSize);
        if (headerSize > limit - start) return fail("Truncated ZIP local header.");
        QByteArray variable;
        if (!read(start + 30, nameSize + extraSize, &variable)) return false;
        if (variable.left(nameSize) != rawName) return fail("ZIP local and central names disagree.");
        Extra extra;
        if (!parseExtra(variable.mid(nameSize), &extra)) return false;
        *unsafeExtra = extra.unsafeType;
        quint64 localSize = u32(local, 22), localCompressed = u32(local, 18);
        bool local64 = localSize == 0xffffffff || localCompressed == 0xffffffff;
        int position = 0;
        if (localSize == 0xffffffff && !take64(extra.zip64, &position, &localSize)) return false;
        if (localCompressed == 0xffffffff && !take64(extra.zip64, &position, &localCompressed)) return false;
        bool descriptor = (member->flags & 8) != 0;
        quint32 crc = u32(local, 14);
        if ((crc != member->crc && (!descriptor || crc != 0)) ||
            (localSize != member->size && (!descriptor || localSize != 0)) ||
            (localCompressed != member->compressed && (!descriptor || localCompressed != 0)))
            return fail("ZIP local and central sizes or checksums disagree.");
        member->offset = start + headerSize;
        if (member->compressed > limit - member->offset) return fail("Truncated ZIP member data.");
        *end = member->offset + member->compressed;
        if (descriptor) {
            bool wide = zip64 || local64;
            int size = wide ? 20 : 12;
            QByteArray data;
            if (quint64(size) > limit - *end || !read(*end, size, &data)) return fail("Truncated ZIP data descriptor.");
            if (u32(data, 0) == 0x08074b50) {
                size += 4;
                if (quint64(size) > limit - *end || !read(*end, size, &data)) return fail("Truncated ZIP data descriptor.");
                data.remove(0, 4);
            }
            if (u32(data, 0) != member->crc || (wide ? u64(data, 4) : u32(data, 4)) != member->compressed ||
                (wide ? u64(data, 12) : u32(data, 8)) != member->size) return fail("ZIP data descriptor disagrees with its directory.");
            *end += quint64(size);
        }
        return true;
    }

    bool readDirectory()
    {
        quint64 offset, length, count;
        if (!directoryBounds(&offset, &length, &count)) return false;
        quint64 position = offset, total = 0;
        QVector<Range> ranges;
        ranges.reserve(int(count));
        for (quint64 index = 0; index < count; ++index) {
            if (stopped()) return false;
            QByteArray header;
            if (position > offset + length || offset + length - position < 46 || !read(position, 46, &header))
                return fail("Truncated ZIP central directory member.");
            if (u32(header, 0) != 0x02014b50) return fail("Invalid ZIP central directory member.");
            int nameSize = u16(header, 28), extraSize = u16(header, 30), commentSize = u16(header, 32);
            quint64 recordSize = quint64(46 + nameSize + extraSize + commentSize);
            if (recordSize > offset + length - position) return fail("Truncated ZIP central directory member.");
            QByteArray variable;
            if (!read(position + 46, nameSize + extraSize, &variable)) return false;
            position += recordSize;
            Member member;
            member.flags = u16(header, 8);
            member.method = u16(header, 10);
            member.crc = u32(header, 16);
            member.compressed = u32(header, 20);
            member.size = u32(header, 24);
            member.offset = u32(header, 42);
            quint64 disk = u16(header, 34);
            bool zip64 = member.size == 0xffffffff || member.compressed == 0xffffffff;
            Extra extra;
            if (!parseExtra(variable.mid(nameSize), &extra)) return false;
            int extraPosition = 0;
            if (member.size == 0xffffffff && !take64(extra.zip64, &extraPosition, &member.size)) return false;
            if (member.compressed == 0xffffffff && !take64(extra.zip64, &extraPosition, &member.compressed)) return false;
            if (member.offset == 0xffffffff && !take64(extra.zip64, &extraPosition, &member.offset)) return false;
            if (disk == 0xffff) {
                if (extraPosition > extra.zip64.size() - 4) return fail("Missing ZIP64 disk number.");
                disk = u32(extra.zip64, extraPosition);
            }
            if (disk) return fail("Split ZIP members are not supported.");
            QByteArray rawName = variable.left(nameSize);
            if (rawName.isEmpty() || rawName.contains('\0')) return fail("Invalid ZIP member name.");
            member.name = decodeName(rawName, (member.flags & 0x800) != 0);
            if (member.name.isEmpty()) return fail("Invalid UTF-8 ZIP member name.");
            member.name.replace('\\', '/');
            quint64 start = member.offset, end = 0;
            bool unsafeExtra = false;
            if (!localRecord(&member, rawName, zip64, offset, &end, &unsafeExtra)) return false;
            ranges.append(Range{start, end});
            // Ignore every other top-level directory, including malformed
            // traversal names in db_extra/db_custom, just as CDIE does.
            if (member.name.section('/', 0, 0) != "db") continue;
            quint32 attributes = u32(header, 38);
            quint16 type = quint16(attributes >> 16) & 0170000;
            bool trailingSlash = member.name.endsWith('/');
            member.directory = trailingSlash || type == 0040000 || (attributes & 0x10);
            if ((type && type != 0040000 && type != 0100000) || (type == 0100000 && member.directory) ||
                (attributes & (0x40 | 0x400)) || extra.unsafeType || unsafeExtra)
                return fail("Database archive contains a link, device, or conflicting file type.");
            if ((member.flags & ~quint16(0x080e)) || (member.method != 0 && member.method != 8) ||
                (member.method == 0 && (member.flags & 6))) return fail("Encrypted or unsupported ZIP database member.");
            if (member.size > MAX_MEMBER || member.size > MAX_EXTRACTED - total)
                return fail("Database archive exceeds the extraction size limits.");
            total += member.size;
            if ((member.directory && (member.size || member.crc)) || (member.method == 0 && member.compressed != member.size))
                return fail("Invalid ZIP directory or stored member size.");
            if (!safeName(&member)) return false;
            m_members.append(member);
        }
        if (position != offset + length) return fail("ZIP central directory size does not match its members.");
        std::sort(ranges.begin(), ranges.end(), [](const Range &left, const Range &right) { return left.begin < right.begin; });
        for (int i = 1; i < ranges.size(); ++i) {
            if (ranges.at(i).begin < ranges.at(i - 1).end) return fail("ZIP member records overlap.");
        }
        return true;
    }

    bool makeDirectories(const QString &name)
    {
        QString path = m_destination;
        const QStringList parts = name.split('/');
        for (const QString &part : parts) {
            if (stopped()) return false;
            path += '/' + part;
            QFileInfo info(path);
            if (info.isSymLink() || (info.exists() && !info.isDir())) return fail("Unsafe staged database directory.");
            if (!info.exists() && !QDir().mkdir(path)) return fail("Cannot create a staged database directory.");
        }
        return true;
    }

    bool unpack(const Member &member, QFile *output)
    {
        if (!m_file.seek(qint64(member.offset))) return fail("Cannot read ZIP member data.");
        quint64 remaining = member.compressed, written = 0;
        uLong crc = crc32(0, Z_NULL, 0);
        z_stream stream;
        std::memset(&stream, 0, sizeof(stream));
        if (member.method == 8 && inflateInit2(&stream, -MAX_WBITS) != Z_OK) return fail("Cannot initialize ZIP decompression.");
        struct InflateGuard {
            z_stream *stream;
            ~InflateGuard() { if (stream) inflateEnd(stream); }
        } guard{member.method == 8 ? &stream : nullptr};
        QByteArray input, buffer(BLOCK_SIZE, '\0');
        bool ended = member.method == 0;
        do {
            if (stopped()) return false;
            if (member.method == 0 || stream.avail_in == 0) {
                int size = int(qMin(remaining, quint64(BLOCK_SIZE)));
                input = m_file.read(size);
                if (input.size() != size) return fail("Truncated ZIP member data.");
                remaining -= quint64(size);
                stream.next_in = reinterpret_cast<Bytef *>(input.data());
                stream.avail_in = uInt(size);
            }
            const char *bytes = input.constData();
            int size = input.size();
            if (member.method == 8) {
                stream.next_out = reinterpret_cast<Bytef *>(buffer.data());
                stream.avail_out = BLOCK_SIZE;
                uInt before = stream.avail_in;
                int status = inflate(&stream, Z_NO_FLUSH);
                size = BLOCK_SIZE - int(stream.avail_out);
                bytes = buffer.constData();
                if (status == Z_STREAM_END) ended = true;
                else if (status != Z_OK || (size == 0 && before == stream.avail_in)) return fail("Invalid or truncated deflated ZIP member.");
            }
            if (quint64(size) > member.size - written) return fail("ZIP member expands beyond its declared size.");
            if (output && output->write(bytes, size) != size) return fail("Cannot write the staged database.");
            crc = crc32(crc, reinterpret_cast<const Bytef *>(bytes), uInt(size));
            written += quint64(size);
            if (member.method == 8 && ended) break;
        } while (remaining || (member.method == 8 && !ended));
        if (!ended || remaining || (member.method == 8 && stream.avail_in) || written != member.size || quint32(crc) != member.crc)
            return fail("ZIP member size or checksum verification failed.");
        return true;
    }

    QFile m_file;
    QString m_destination;
    QString *m_error;
    const std::function<bool()> &m_cancelled;
    quint64 m_size = 0;
    DatabaseArchive::Result m_result = DatabaseArchive::Invalid;
    bool m_createdDatabase = false;
    bool m_success = false;
    QVector<Member> m_members;
    QHash<QString, QString> m_paths;
    QSet<QString> m_directories;
    QSet<QString> m_explicit;
};
}

DatabaseArchive::Result DatabaseArchive::extractAndValidate(const QString &archivePath, const QString &destinationRoot, int *signatures, QString *error,
                                                           const std::function<bool()> &cancelled)
{
    if (signatures) *signatures = 0;
    if (error) error->clear();
    Extractor extractor(archivePath, destinationRoot, error, cancelled);
    return extractor.extract(signatures);
}
