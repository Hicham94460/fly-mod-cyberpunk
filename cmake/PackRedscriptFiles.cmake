string(TIMESTAMP CURRENT_TIMESTAMP "%Y-%m-%dT%H:%M:%SZ UTC" UTC)

file(WRITE ${LTBF_REDSCRIPT_PACKED_FILE} "// Generated on ${CURRENT_TIMESTAMP}\n\n")
file(GLOB_RECURSE REDSCRIPT_FILES ${LTBF_REDSCRIPT_DIR}/*.reds)

function(add_redscript_file IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(RELATIVE_PATH IN_FILE_RELATIVE ${LTBF_REDSCRIPT_DIR} ${IN_FILE})
  file(APPEND ${OUT_FILE} "// ${IN_FILE_RELATIVE}\n\n")
  message(STATUS "${IN_FILE_RELATIVE}")
  file(APPEND ${OUT_FILE} "${CONTENTS}\n\n")
endfunction()

foreach(REDSCRIPT_FILE ${REDSCRIPT_FILES})
  add_redscript_file(${REDSCRIPT_FILE} ${LTBF_REDSCRIPT_PACKED_FILE})
endforeach()