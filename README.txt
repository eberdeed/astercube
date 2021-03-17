

    astercube:  A Program to display an asteroid field of instanced asteroids using
    the SimpleDirect Media Layer and OpenGL.
    
    This program requires: 
    The assimp library www.assimp.org, 
    the freeimageplus library www.sourceforge.net,
    GLEW http://glew.sourceforge.net, and
    CMAKE www.cmake.org.
    SDL2 https://wiki.libsdl.org/FrontPage
    the boost library  www.boost.org, 
    Doxygen http://www.doxygen.nl
    GraphViz https://www.graphviz.org for the "dot" program used by doxygen.
    pthread https://www.gnu.org/software/hurd/libpthread.html
    OpenGL is generally available on any system as part 
    of the underlying graphics subsystem and GLEW will 
    get you access to it, X11 used for window sizing is
    part of the X windowing system used on most Linux systems.
    
    To compile the program:
    
    be super user or su
    ./compile.sh
    
    to clean the program:
    
    .clean.sh
    
    To run the program:
    
    astercube
    
    The key layout is as follows:

    wasd as usual motion keys.
    
    Viewing direction:
    Mouse: In order to change view direction mouse down to select start move and then mouse up to stop motion.
    
    x reverse view.
    z reset view.
    Escape ends the program.
    
    Mouse wheel forward zooms in.
    Mouse wheel back zooms out.
    
    wasd are the usual movement keys.
    w forward.
    a left.
    s backward.
    d right.
    
    alt-enter will put you in fullscreen mode.
    
    Documentation is available after install at:
    
    /usr/share/doc/astecube-doc
    
        
    The program was written and compiled on debian linux available at:  www.debian.org.
    
    Limitations of the underlying assimpopengl library are:
    
    You must use separate instances if you have instanced and non-instanced objects.
    
    You must use separate instances and separate shaders if your instanced objects
    have different instance quantities of images produced.
    
    Otherwise you are free to provide multiple objects to the model class and 
    have them displayed.
    
    Note also that the universe has a -y direction as up.
    
    You can reach me at <eberdeed@eberdeed.net>.
    
    Edward Charles Eberle
    San Diego, California United States of America
    
    This program is licensed under the Lesser GNU Public license.
    It can be found in the assimpopengl/LGPL directory 
    and once installed it will be also found at 
    /usr/share/doc/astecube-doc/LGPL.  This 
    program is given as an instructional aid for learning
    and using the libraries presented and is not to 
    be considered fit for any particular use.
