Windows
=======
Using VS, add lib and include listed in external_libs

Mac
===
```bash
g++ -std=c++11 -W -Wall -Wextra -pedantic -I external_libs/include/ ./*.cpp -o game -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon -lGLEW
```

TODO
----
Add instructions for glfw3, glm, and glew
