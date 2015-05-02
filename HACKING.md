## Introduction

This page details the development policies of the project.

## Source code

### Code conventions

We will use the [Qt Coding Style](https://wiki.qt.io/Qt_Coding_Style). If you
want to use some feature that is not covered by the previous mentioned document,
check the [complementary document](https://wiki.qt.io/Coding_Conventions) or
what
[Google thinks about it](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml).
The following rules take precedence over these documents:

- Exceptions are allowed
- RTTI is allowed
- UTF-8 only
- Don't use Google naming convention (Qt is preferred)
- If you have property `foobar` in your class:
  - You **should** use `foobar_` to name the variable
  - You **must** use `foobar()` to name the getter, if exists
  - You **must** use `setFoobar()` to name the setter, if exists
- Public classes don't need to start with a `Q` (e.g. `QRgb`)
- C++11 features are okay

## Art

### File formats

- Vector graphics **must** be in svg format
- Raster image files **must** be in png format
- Sound files **must** be in one of these formats:
  - Vorbis
  - Opus
  - FLAC
- Video files **must** be in one of these fomats:
  - Theora
  - VP8
- The container format for the media (audio and video) files **must** be one of
  the following formats:
  - Ogg
  - Matroska/WebM
- The package format **should** be
  [Qt's rcc](http://doc.qt.nokia.com/4.7-snapshot/resources.html#external-binary-resources)

### Resolution/ratio

The game **must** use vector-based images, leading it to not be resolution
dependent. The gameplay scenario **must** be _tile-based_. All tiles **must** be
in a _1:1 ratio_ (_width:height_). Objects to be layered on the top of the tiles
can be _1:1 or 1:2 ratio_. The objects layer **must** use a
[Top-down view](http://en.wikipedia.org/wiki/Top-down_perspective#Top-down_perspective).

### Style

The decorative graphics **should** be in
[chibi style](http://en.wikipedia.org/wiki/Super_deformed).
