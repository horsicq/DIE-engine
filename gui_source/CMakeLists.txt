cmake_minimum_required(VERSION 3.5)

project(DIE VERSION 3.03 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY ../release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../release)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Concurrent REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Svg REQUIRED)

add_definitions(-DUSE_DEX)
add_definitions(-DUSE_ARCHIVE)

include_directories(${PROJECT_SOURCE_DIR}/../Controls/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatDialogs/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/Binary/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/DEX/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/ELF/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/LE/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/MACH/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/MACHOFAT/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/MSDOS/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/MultiSearch/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/NE/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/PE/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/SearchSignatures/)
include_directories(${PROJECT_SOURCE_DIR}/../FormatWidgets/SearchStrings/)
include_directories(${PROJECT_SOURCE_DIR}/../Formats/)
include_directories(${PROJECT_SOURCE_DIR}/../SpecAbstract/)
include_directories(${PROJECT_SOURCE_DIR}/../StaticScan/)
include_directories(${PROJECT_SOURCE_DIR}/../XArchive/)
include_directories(${PROJECT_SOURCE_DIR}/../XArchive/3rdparty/bzip2/src/)
include_directories(${PROJECT_SOURCE_DIR}/../XArchive/3rdparty/lzma/src/)
include_directories(${PROJECT_SOURCE_DIR}/../XArchive/3rdparty/zlib/src/)
include_directories(${PROJECT_SOURCE_DIR}/../XCapstone/)
include_directories(${PROJECT_SOURCE_DIR}/../XCapstone/3rdparty/Capstone/src/include/)
include_directories(${PROJECT_SOURCE_DIR}/../XCppfilt/)
include_directories(${PROJECT_SOURCE_DIR}/../XCppfilt/3rdparty/cppfilt/src/)
include_directories(${PROJECT_SOURCE_DIR}/../XDEX/)
include_directories(${PROJECT_SOURCE_DIR}/../XDemangle/)
include_directories(${PROJECT_SOURCE_DIR}/../XDemangleWidget/)
include_directories(${PROJECT_SOURCE_DIR}/../XDisasmView/)
include_directories(${PROJECT_SOURCE_DIR}/../XEntropyWidget/)
include_directories(${PROJECT_SOURCE_DIR}/../XHashWidget/)
include_directories(${PROJECT_SOURCE_DIR}/../XHexView/)
include_directories(${PROJECT_SOURCE_DIR}/../XLLVMDemangler/3rdparty/)
include_directories(${PROJECT_SOURCE_DIR}/../XLLVMDemangler/3rdparty/llvm/Demangle/)
include_directories(${PROJECT_SOURCE_DIR}/../XMemoryMapWidget/)
include_directories(${PROJECT_SOURCE_DIR}/../XOptions/)
include_directories(${PROJECT_SOURCE_DIR}/../XQwt/3rdparty/qwt/src/)
include_directories(${PROJECT_SOURCE_DIR}/../XShortcuts/)
include_directories(${PROJECT_SOURCE_DIR}/../XMIMEWidget/)

set(PROJECT_SOURCES
            ../Controls/hexvalidator.cpp
            ../Controls/subclassofqstyleditemdelegate.cpp
            ../Controls/xabstracttableview.cpp
            ../Controls/xcomboboxex.cpp
            ../Controls/xdatetimeeditx.cpp
            ../Controls/xdevicetableview.cpp
            ../Controls/xlineedithex.cpp
            ../FormatDialogs/dialogdump.ui
            ../FormatDialogs/dialogdumpprocess.cpp
            ../FormatDialogs/dialogdumpprocess.ui
            ../FormatDialogs/dialoggotoaddress.cpp
            ../FormatDialogs/dialoggotoaddress.ui
            ../FormatDialogs/dialoghexsignature.cpp
            ../FormatDialogs/dialoghexsignature.ui
            ../FormatDialogs/dialoginfo.cpp
            ../FormatDialogs/dialoginfo.ui
            ../FormatDialogs/dialogsearch.cpp
            ../FormatDialogs/dialogsearch.ui
            ../FormatDialogs/dialogsearchprocess.cpp
            ../FormatDialogs/dialogsearchprocess.ui
            ../FormatDialogs/dialogtextinfo.cpp
            ../FormatDialogs/dialogtextinfo.ui
            ../FormatDialogs/dumpprocess.cpp
            ../FormatDialogs/searchprocess.cpp
            ../FormatWidgets/MultiSearch/dialogmultisearchprocess.cpp
            ../FormatWidgets/MultiSearch/dialogmultisearchprocess.ui
            ../FormatWidgets/MultiSearch/multisearch.cpp
            ../FormatWidgets/PE/dialogpe.cpp
            ../FormatWidgets/PE/dialogpe.ui
            ../FormatWidgets/PE/pe_defs.cpp
            ../FormatWidgets/PE/peprocessdata.cpp
            ../FormatWidgets/PE/pesectionheaderwidget.cpp
            ../FormatWidgets/PE/pesectionheaderwidget.ui
            ../FormatWidgets/PE/pewidget.cpp
            ../FormatWidgets/PE/pewidget.ui
            ../FormatWidgets/SearchSignatures/dialogsearchsignatures.cpp
            ../FormatWidgets/SearchSignatures/dialogsearchsignatures.ui
            ../FormatWidgets/SearchSignatures/searchsignatureswidget.cpp
            ../FormatWidgets/SearchSignatures/searchsignatureswidget.ui
            ../FormatWidgets/SearchStrings/dialogsearchstrings.cpp
            ../FormatWidgets/SearchStrings/dialogsearchstrings.ui
            ../FormatWidgets/SearchStrings/searchstringswidget.cpp
            ../FormatWidgets/SearchStrings/searchstringswidget.ui
            ../FormatWidgets/dialogmodelinfo.cpp
            ../FormatWidgets/dialogmodelinfo.ui
            ../FormatWidgets/dialogprocessdata.cpp
            ../FormatWidgets/dialogprocessdata.ui
            ../FormatWidgets/dialogsectionheader.cpp
            ../FormatWidgets/dialogsectionheader.ui
            ../FormatWidgets/formatwidget.cpp
            ../FormatWidgets/invwidget.cpp
            ../FormatWidgets/processdata.cpp
            ../FormatWidgets/toolswidget.cpp
            ../FormatWidgets/toolswidget.ui
            ../Formats/subdevice.cpp
            ../Formats/xbinary.cpp
            ../Formats/xcom.cpp
            ../Formats/xelf.cpp
            ../Formats/xformats.cpp
            ../Formats/xle.cpp
            ../Formats/xmach.cpp
            ../Formats/xmsdos.cpp
            ../Formats/xne.cpp
            ../Formats/xpe.cpp
            ../SpecAbstract/specabstract.cpp
            ../StaticScan/dialogstaticscan.cpp
            ../StaticScan/dialogstaticscan.ui
            ../StaticScan/dialogstaticscanprocess.cpp
            ../StaticScan/dialogstaticscanprocess.ui
            ../StaticScan/formresult.cpp
            ../StaticScan/formresult.ui
            ../StaticScan/heurwidget.cpp
            ../StaticScan/heurwidget.ui
            ../StaticScan/staticscan.cpp
            ../StaticScan/staticscanitem.cpp
            ../StaticScan/staticscanitemmodel.cpp
            ../XArchive/xarchive.cpp
            ../XArchive/xarchives.cpp
            ../XArchive/xcab.cpp
            ../XArchive/xmachofat.cpp
            ../XArchive/xrar.cpp
            ../XArchive/xsevenzip.cpp
            ../XArchive/xzip.cpp
            ../XCapstone/xcapstone.cpp
            ../XCppfilt/xcppfilt.cpp
            ../XDEX/xandroidbinary.cpp
            ../XDEX/xdex.cpp
            ../XDemangle/xdemangle.cpp
            ../XDemangleWidget/dialogdemangle.cpp
            ../XDemangleWidget/dialogdemangle.ui
            ../XDemangleWidget/xdemanglewidget.cpp
            ../XDemangleWidget/xdemanglewidget.ui
            ../XDisasmView/dialogmultidisasm.cpp
            ../XDisasmView/dialogmultidisasm.ui
            ../XDisasmView/dialogmultidisasmsignature.cpp
            ../XDisasmView/dialogmultidisasmsignature.ui
            ../XDisasmView/xdisasmview.cpp
            ../XDisasmView/xmultidisasmwidget.cpp
            ../XDisasmView/xmultidisasmwidget.ui
            ../XEntropyWidget/dialogentropy.cpp
            ../XEntropyWidget/dialogentropy.ui
            ../XEntropyWidget/dialogentropyprocess.cpp
            ../XEntropyWidget/dialogentropyprocess.ui
            ../XEntropyWidget/entropyprocess.cpp
            ../XEntropyWidget/xentropywidget.cpp
            ../XEntropyWidget/xentropywidget.ui
            ../XHashWidget/dialoghash.cpp
            ../XHashWidget/dialoghash.ui
            ../XHashWidget/dialoghashprocess.cpp
            ../XHashWidget/dialoghashprocess.ui
            ../XHashWidget/hashprocess.cpp
            ../XHashWidget/xhashwidget.cpp
            ../XHashWidget/xhashwidget.ui
            ../XHexView/dialoghexview.cpp
            ../XHexView/dialoghexview.ui
            ../XHexView/xhexview.cpp
            ../XHexView/xhexviewwidget.cpp
            ../XHexView/xhexviewwidget.ui
            ../XMemoryMapWidget/dialogmemorymap.cpp
            ../XMemoryMapWidget/dialogmemorymap.ui
            ../XMemoryMapWidget/xmemorymapwidget.cpp
            ../XMemoryMapWidget/xmemorymapwidget.ui
            ../XOptions/xoptions.cpp
            ../XQwt/3rdparty/qwt/src/qwt_abstract_legend.cpp
            ../XQwt/3rdparty/qwt/src/qwt_abstract_scale.cpp
            ../XQwt/3rdparty/qwt/src/qwt_abstract_scale_draw.cpp
            ../XQwt/3rdparty/qwt/src/qwt_abstract_slider.cpp
            ../XQwt/3rdparty/qwt/src/qwt_analog_clock.cpp
            ../XQwt/3rdparty/qwt/src/qwt_arrow_button.cpp
            ../XQwt/3rdparty/qwt/src/qwt_clipper.cpp
            ../XQwt/3rdparty/qwt/src/qwt_color_map.cpp
            ../XQwt/3rdparty/qwt/src/qwt_column_symbol.cpp
            ../XQwt/3rdparty/qwt/src/qwt_compass.cpp
            ../XQwt/3rdparty/qwt/src/qwt_compass_rose.cpp
            ../XQwt/3rdparty/qwt/src/qwt_counter.cpp
            ../XQwt/3rdparty/qwt/src/qwt_curve_fitter.cpp
            ../XQwt/3rdparty/qwt/src/qwt_date.cpp
            ../XQwt/3rdparty/qwt/src/qwt_date_scale_draw.cpp
            ../XQwt/3rdparty/qwt/src/qwt_date_scale_engine.cpp
            ../XQwt/3rdparty/qwt/src/qwt_dial.cpp
            ../XQwt/3rdparty/qwt/src/qwt_dial_needle.cpp
            ../XQwt/3rdparty/qwt/src/qwt_dyngrid_layout.cpp
            ../XQwt/3rdparty/qwt/src/qwt_event_pattern.cpp
            ../XQwt/3rdparty/qwt/src/qwt_graphic.cpp
            ../XQwt/3rdparty/qwt/src/qwt_interval.cpp
            ../XQwt/3rdparty/qwt/src/qwt_interval_symbol.cpp
            ../XQwt/3rdparty/qwt/src/qwt_knob.cpp
            ../XQwt/3rdparty/qwt/src/qwt_legend.cpp
            ../XQwt/3rdparty/qwt/src/qwt_legend_data.cpp
            ../XQwt/3rdparty/qwt/src/qwt_legend_label.cpp
            ../XQwt/3rdparty/qwt/src/qwt_magnifier.cpp
            ../XQwt/3rdparty/qwt/src/qwt_math.cpp
            ../XQwt/3rdparty/qwt/src/qwt_matrix_raster_data.cpp
            ../XQwt/3rdparty/qwt/src/qwt_null_paintdevice.cpp
            ../XQwt/3rdparty/qwt/src/qwt_painter.cpp
            ../XQwt/3rdparty/qwt/src/qwt_painter_command.cpp
            ../XQwt/3rdparty/qwt/src/qwt_panner.cpp
            ../XQwt/3rdparty/qwt/src/qwt_picker.cpp
            ../XQwt/3rdparty/qwt/src/qwt_picker_machine.cpp
            ../XQwt/3rdparty/qwt/src/qwt_pixel_matrix.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_abstract_barchart.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_axis.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_barchart.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_canvas.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_curve.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_dict.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_directpainter.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_glcanvas.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_grid.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_histogram.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_intervalcurve.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_item.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_layout.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_legenditem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_magnifier.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_marker.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_multi_barchart.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_panner.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_picker.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_rasteritem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_renderer.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_rescaler.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_scaleitem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_seriesitem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_shapeitem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_spectrocurve.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_spectrogram.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_svgitem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_textlabel.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_tradingcurve.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_xml.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_zoneitem.cpp
            ../XQwt/3rdparty/qwt/src/qwt_plot_zoomer.cpp
            ../XQwt/3rdparty/qwt/src/qwt_point_3d.cpp
            ../XQwt/3rdparty/qwt/src/qwt_point_data.cpp
            ../XQwt/3rdparty/qwt/src/qwt_point_mapper.cpp
            ../XQwt/3rdparty/qwt/src/qwt_point_polar.cpp
            ../XQwt/3rdparty/qwt/src/qwt_raster_data.cpp
            ../XQwt/3rdparty/qwt/src/qwt_round_scale_draw.cpp
            ../XQwt/3rdparty/qwt/src/qwt_sampling_thread.cpp
            ../XQwt/3rdparty/qwt/src/qwt_scale_div.cpp
            ../XQwt/3rdparty/qwt/src/qwt_scale_draw.cpp
            ../XQwt/3rdparty/qwt/src/qwt_scale_engine.cpp
            ../XQwt/3rdparty/qwt/src/qwt_scale_map.cpp
            ../XQwt/3rdparty/qwt/src/qwt_scale_widget.cpp
            ../XQwt/3rdparty/qwt/src/qwt_series_data.cpp
            ../XQwt/3rdparty/qwt/src/qwt_slider.cpp
            ../XQwt/3rdparty/qwt/src/qwt_spline.cpp
            ../XQwt/3rdparty/qwt/src/qwt_symbol.cpp
            ../XQwt/3rdparty/qwt/src/qwt_system_clock.cpp
            ../XQwt/3rdparty/qwt/src/qwt_text.cpp
            ../XQwt/3rdparty/qwt/src/qwt_text_engine.cpp
            ../XQwt/3rdparty/qwt/src/qwt_text_label.cpp
            ../XQwt/3rdparty/qwt/src/qwt_thermo.cpp
            ../XQwt/3rdparty/qwt/src/qwt_transform.cpp
            ../XQwt/3rdparty/qwt/src/qwt_wheel.cpp
            ../XQwt/3rdparty/qwt/src/qwt_widget_overlay.cpp
            ../XShortcuts/dialogshortcuts.cpp
            ../XShortcuts/dialogshortcuts.ui
            ../XShortcuts/xshortcuts.cpp
            ../XShortcuts/xshortcutsdialog.cpp
            ../XShortcuts/xshortcutstscrollarea.cpp
            ../XShortcuts/xshortcutswidget.cpp
            dialogabout.cpp
            dialogabout.ui
            dialogoptions.cpp
            dialogoptions.ui
            guimainwindow.cpp
            guimainwindow.ui
            main_gui.cpp
            res.qrc
)

if(APPLE)
    add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER ntinfo.die
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            BUNDLE TRUE)
    set(MACOSX_BUNDLE_ICON_FILE ../icons/main.icns)
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(${PROJECT_NAME} bzip2)
target_link_libraries(${PROJECT_NAME} lzma)
target_link_libraries(${PROJECT_NAME} zlib)
target_link_libraries(${PROJECT_NAME} capstone)
target_link_libraries(${PROJECT_NAME} cppfilt)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Concurrent)
target_link_libraries(${PROJECT_NAME} Qt5::OpenGL)
target_link_libraries(${PROJECT_NAME} Qt5::PrintSupport)
target_link_libraries(${PROJECT_NAME} Qt5::Svg)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} Wintrust)
    target_link_libraries(${PROJECT_NAME} Crypt32)
endif()
