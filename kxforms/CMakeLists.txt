
add_subdirectory( tests )

########### next target ###############

set( kxforms_SRCS
   main.cpp
   mainwindow.cpp
   manager.cpp 
   form.cpp
   formcreator.cpp
   hints.cpp
   list.cpp
   listmodel.cpp
   input.cpp
   formgui.cpp 
   remotefile.cpp
   xformscommon.cpp
   xmlbuilder.cpp
   guielement.cpp
   kresult.cpp
   guihandler.cpp
   guihandlerflat.cpp
   guihandlerdialogs.cpp
   reference.cpp
   textarea.cpp
   select1.cpp
   section.cpp
   dispatcher.cpp
   select.cpp
   listproxymodel.cpp
   layout.cpp
   editor/editorwidget.cpp
   editor/editor.cpp
   editor/editoraction.cpp
   editor/appearanceaction.cpp
   editor/listaction.cpp
   editor/positionaction.cpp
   editor/layoutstyleaction.cpp
   editor/groupaction.cpp
   editor/changelabelaction.cpp
   editor/readonlyaction.cpp
   editor/inputtypeaction.cpp
   editor/globalsettingsdlg.cpp )


kde4_add_kcfg_files( kxforms_SRCS prefs.kcfgc )

kde4_add_executable( kxforms ${kxforms_SRCS} )

target_link_libraries( kxforms ${KDE4_KIO_LIBS} kschema ${KDE4_KDE3SUPPORT_LIBS} )

install( TARGETS kxforms ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( FILES kxformsui.rc DESTINATION ${DATA_INSTALL_DIR}/kxforms )

install( FILES kxforms.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
