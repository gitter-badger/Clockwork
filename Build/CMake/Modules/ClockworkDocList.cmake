# create target to generate docs with clang based doc tool
find_program(CLDOC cldoc)
if(CLDOC)
   add_custom_target(ClockworkEngineDocs
    DEPENDS ClockworkEditorDocs ClockworkDocs
    )
endif()
