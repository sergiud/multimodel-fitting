set(EXAMPLES
    hello_world  
)

foreach(EXAMPLE ${EXAMPLES})
  add_subdirectory(${EXAMPLE})
endforeach(EXAMPLE)