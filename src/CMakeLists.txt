set(HEADERS
  ${multimodelfitting_HEADERS}
  # internal headers, not for external use:
  # todo.
)

set(SOURCES
  multimodelfitting.cxx
)

add_library(multimodelfitting ${SOURCES} ${HEADERS})