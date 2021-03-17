
    libassimpopengl.so  A shared library to display arbitrary blender objects.
    
    To use the library include assimpopengl/assimpopengl.h
    and link to assimpopengl
    
    The include file will be at:
    /usr/include/assimpopengl/assimpopengl.h
    
    The library binary will be at:
    /usr/lib/libassimpopengl.so
    
    For info on the interface look at:
    /usr/share/doc/assimpopengl-doc/html/index.html
    and examine the model class which is the 
    entry point.

    to build the library on its own:
    First go through the cmake file and uncomment out the bottom several lines
    (remove the "#" symbols).
    
    cd build
    cmake ..
    sudo make assimpdocs
    sudo make install
    
        
    The program was written and compiled on debian linux available at:  www.debian.org.
    
    You can reach me at <eberdeed@eberdeed.net>.
    
    The sources I used to educate myself concerning OpenGL are
    as follows:
        "OpenGL ES 3.0 Programming Guide Second Edition" 
    by Dan Ginsburg and Budirijanto Purnomo published by Addison-Wesley 2014.
        www.learnopengl.com.
    GLEW http://glew.sourceforge.net, and
    CMAKE http://www.cmake.org.
    Doxygen http://www.doxygen.nl
    GraphViz https://www.graphviz.org
    Assimp https://www.assimp.org
    OpenGL is generally available on any system as part 
    of the underlying graphics subsystem and GLEW will 
    get you access to it.
    
    The program was written and compiled on debian linux available at:  www.debian.org.

    
    Edward Charles Eberle
    April 17th, 2020 San Diego, California United States of America
    
    This program is licensed under the Lesser GNU Public license.
    It can be found in the LGPL directory 
    and once installed it will be also found at 
    /usr/share/doc/assimpopengl-doc/LGPL.  This 
    program is given as an instructional aid for learning
    and using the libraries presented and is not to 
    be considered fit for any particular use.
