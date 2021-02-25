# JCTerminal

# Dependencies

- glfw 
- glad 
- glm 
- freetype 
- SAELib 
 

Of the above dependencies, `glad` and `lodepng` are included with the library due to the lack of cmake support.
The rest of the dependencies will be automatically cloned into the library's `lib/` directory if they are not already defined and option
`JCTERMINAL_GIT_CLONE_DEPENDENCIES`is set to `ON`.
