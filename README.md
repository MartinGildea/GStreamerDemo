# GStreamer Demo Project
This project was built in Microsoft Visual Studio using C++ 20 in a "Windows 10 Home" environment. Minor changes may be required when running on other environments and operating systems.

### Pre-requisites and Installation.
These installation steps are also exclusively for a a Visual Studio environment on a Windows 10 Home computer.

1. GStreamer must be installed on your system with the "gst-plugins-bad" plugin in order to use the encoder. The default installation directory is assumed for steps 2-3.

2. The following string should be added to your Additional Include directories under Configuration Properties - C/C++ - General:
   
     ```C:\gstreamer\1.0\msvc_x86_64\lib\glib-2.0\include; C:\gstreamer\1.0\msvc_x86_64\include\gstreamer-1.0; C:\gstreamer\1.0\msvc_x86_64\include\glib-2.0;```

3. The following string should be added to your Additional Library directories under Configuration Properties - Linker - General:
   
     ```C:\gstreamer\1.0\msvc_x86_64\lib\glib-2.0\include; C:\gstreamer\1.0\msvc_x86_64\include\gstreamer-1.0; C:\gstreamer\1.0\msvc_x86_64\include\glib-2.0;```

4. The following string should be added to your Additional Dependencies under Configuration Properties - Linker - Input:
   
     ```gobject-2.0.lib; glib-2.0.lib; gstreamer-1.0.lib;```

### Running the Program
Once the program is installed, the program can be run as a standard C++ project after compiling it.

It will stream input from the host's default camera and then save an MP4 file to disk every 60 seconds.

The default save location is "C:/GStreamerDemo/output/" but this can be changed by modifying the kOutputDirectory constant.


### Possible Expansions

1. The program does not handle abrupt closures of the application, signal functionality could be used to detect when the application closes and cleanly cut the final video.

2. The program could be made to be platform-independent but has been made with a Windows operating system in mind.

3. More robust error handling could be provided with a more granular approach, identifying additional exceptions and edge cases and handling those that exist in a better way than printing to the console.
