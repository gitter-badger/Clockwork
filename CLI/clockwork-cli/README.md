# clockwork-cli

Command line tools for the [Clockwork Game Engine](http://www.clockwork.sunwells.net)

##### Website

<a href="http://www.clockwork.sunwells.net">http://www.clockwork.sunwells.net</a>
<!-- 
##### Chat

[![Join the chat at https://gitter.im/ClockworkGameEngine/ClockworkGameEngine](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/ClockworkGameEngine/ClockworkGameEngine?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge) -->

<!-- ##### Forums

<a href="http://clockworkgameengine.com/forum">Clockwork Game Engine Forums</a>

##### Twitter

<a href="https://twitter.com/ClockworkGEngine">Follow @ClockworkGEngine</a>

[WelcomeScreen]: https://github.com/ClockworkGameEngine/ClockworkExamples/wiki/images/WelcomeScreen.png

![alt text][WelcomeScreen] -->

# Install

clockwork-cli must be installed and activated

```
npm install clockwork-cli -g
clockwork-cli activate
```

# usage

```
Usage: clockwork-cli

  Commands:

    activate                  activate
    deactivate                deactivates
    new <folder>              creates project in the specified folder
    add <platform>            adds a platform to the project
                              (mac|windows|ios|android|web)
    run [options] <platform>  builds and runs the project on a specified platform
    edit [path_to_project]    opens the Clockwork Editor loading specified project

  Options:

    -h, --help     output usage information
    -V, --version  output the version number
```
