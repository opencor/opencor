PROJECT(QwtPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_QWT_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.dll 734385d95eb0b8a425a62d7fd45ff32e6b6c481a)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.lib 464758a845a84319a92a1e3a0ea2207d53a7198f)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.dll d6f148d51f5ae884797b95a3898089fd2b503c4b)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.lib 370b8063f0e96cb4a2ced3080a95fa7c14e7dbab)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQwt.dylib 7a1275af59154387cd4138bb21dd2682f500ee07)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQwt.so 455e9ad877490c53b1c1b849eaedb3015ee3a123)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQwt.so 39149f96685869e25afca1e4d6fd48199c551afd)
        ENDIF()
    ENDIF()
ELSE()
    # Add the plugin

    ADD_PLUGIN(Qwt
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/qwt_abstract_legend.cpp
            src/qwt_abstract_scale.cpp
            src/qwt_abstract_scale_draw.cpp
            src/qwt_abstract_slider.cpp
            src/qwt_analog_clock.cpp
            src/qwt_arrow_button.cpp
            src/qwt_clipper.cpp
            src/qwt_color_map.cpp
            src/qwt_column_symbol.cpp
            src/qwt_compass.cpp
            src/qwt_compass_rose.cpp
            src/qwt_counter.cpp
            src/qwt_curve_fitter.cpp
            src/qwt_date.cpp
            src/qwt_date_scale_draw.cpp
            src/qwt_date_scale_engine.cpp
            src/qwt_dial.cpp
            src/qwt_dial_needle.cpp
            src/qwt_dyngrid_layout.cpp
            src/qwt_event_pattern.cpp
            src/qwt_graphic.cpp
            src/qwt_interval.cpp
            src/qwt_interval_symbol.cpp
            src/qwt_knob.cpp
            src/qwt_legend.cpp
            src/qwt_legend_data.cpp
            src/qwt_legend_label.cpp
            src/qwt_magnifier.cpp
            src/qwt_math.cpp
            src/qwt_matrix_raster_data.cpp
            src/qwt_null_paintdevice.cpp
            src/qwt_painter.cpp
            src/qwt_painter_command.cpp
            src/qwt_panner.cpp
            src/qwt_picker.cpp
            src/qwt_picker_machine.cpp
            src/qwt_pixel_matrix.cpp
            src/qwt_plot.cpp
            src/qwt_plot_abstract_barchart.cpp
            src/qwt_plot_axis.cpp
            src/qwt_plot_barchart.cpp
            src/qwt_plot_canvas.cpp
            src/qwt_plot_curve.cpp
            src/qwt_plot_dict.cpp
            src/qwt_plot_directpainter.cpp
            src/qwt_plot_glcanvas.cpp
            src/qwt_plot_grid.cpp
            src/qwt_plot_histogram.cpp
            src/qwt_plot_intervalcurve.cpp
            src/qwt_plot_item.cpp
            src/qwt_plot_layout.cpp
            src/qwt_plot_legenditem.cpp
            src/qwt_plot_magnifier.cpp
            src/qwt_plot_marker.cpp
            src/qwt_plot_multi_barchart.cpp
            src/qwt_plot_panner.cpp
            src/qwt_plot_picker.cpp
            src/qwt_plot_rasteritem.cpp
            src/qwt_plot_renderer.cpp
            src/qwt_plot_rescaler.cpp
            src/qwt_plot_scaleitem.cpp
            src/qwt_plot_seriesitem.cpp
            src/qwt_plot_shapeitem.cpp
            src/qwt_plot_spectrocurve.cpp
            src/qwt_plot_spectrogram.cpp
            src/qwt_plot_svgitem.cpp
            src/qwt_plot_textlabel.cpp
            src/qwt_plot_tradingcurve.cpp
            src/qwt_plot_xml.cpp
            src/qwt_plot_zoneitem.cpp
            src/qwt_plot_zoomer.cpp
            src/qwt_point_3d.cpp
            src/qwt_point_data.cpp
            src/qwt_point_mapper.cpp
            src/qwt_point_polar.cpp
            src/qwt_raster_data.cpp
            src/qwt_round_scale_draw.cpp
            src/qwt_sampling_thread.cpp
            src/qwt_scale_div.cpp
            src/qwt_scale_draw.cpp
            src/qwt_scale_engine.cpp
            src/qwt_scale_map.cpp
            src/qwt_scale_widget.cpp
            src/qwt_series_data.cpp
            src/qwt_slider.cpp
            src/qwt_spline.cpp
            src/qwt_symbol.cpp
            src/qwt_system_clock.cpp
            src/qwt_text.cpp
            src/qwt_text_engine.cpp
            src/qwt_text_label.cpp
            src/qwt_thermo.cpp
            src/qwt_transform.cpp
            src/qwt_wheel.cpp
            src/qwt_widget_overlay.cpp
            src/qwtplugin.cpp

            textengines/mathml/qwt_mathml_text_engine.cpp
            textengines/mathml/qwt_mml_document.cpp
            textengines/mathml/qwt_mml_entity_table.cpp
        HEADERS_MOC
            src/qwt_abstract_legend.h
            src/qwt_abstract_scale.h
            src/qwt_abstract_slider.h
            src/qwt_analog_clock.h
            src/qwt_compass.h
            src/qwt_counter.h
            src/qwt_dial.h
            src/qwt_dyngrid_layout.h
            src/qwt_knob.h
            src/qwt_legend.h
            src/qwt_legend_label.h
            src/qwt_magnifier.h
            src/qwt_panner.h
            src/qwt_picker.h
            src/qwt_plot.h
            src/qwt_plot_canvas.h
            src/qwt_plot_glcanvas.h
            src/qwt_plot_magnifier.h
            src/qwt_plot_panner.h
            src/qwt_plot_picker.h
            src/qwt_plot_renderer.h
            src/qwt_plot_zoomer.h
            src/qwt_sampling_thread.h
            src/qwt_scale_widget.h
            src/qwt_slider.h
            src/qwt_text_label.h
            src/qwt_thermo.h
            src/qwt_wheel.h
            src/qwtplugin.h
        INCLUDE_DIRS
            src
            textengines/mathml
        DEFINITIONS
            QWT_DLL
            QWT_MAKEDLL
        QT_MODULES
            Concurrent
            OpenGL
            PrintSupport
            Svg
            Widgets
            Xml
        QT_LIBRARIES
            QtConcurrent
            QtCore
            QtGui
            QtOpenGL
            QtPrintSupport
            QtSvg
            QtWidgets
            QtXml
    )
ENDIF()
