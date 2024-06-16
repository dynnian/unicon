# unicon

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Bulding](#building)
- [Usage](#usage)
- [Options](#options)
- [Examples](#examples)

## Introduction

**unicon** is a command-line unit conversion tool that allows you to easily
convert values between various units such as temperature, length, time, mass,
and digital storage. This tool is designed to be user-friendly and flexible,
allowing you to convert units with precision.

## Features

- Convert between a wide range of unit types including temperature, length,
  time, mass, and digital storage.
- Easily specify the number of decimal places for rounding the result.

## Building

This tool is provided as source code and can be built using `make`. By just
typing make in the terminal within the code's directory:

```bash
make
```

You should now have an executable named `unicon`. You can copy it to a location
in your PATH for easy access.

## Usage

The general usage format for the **unicon** tool is as follows:

```bash
unicon [OPTIONS] VALUE from <UNIT> to <UNIT>
```

- `OPTIONS`: Optional command-line options (see [Options](#options)).
- `VALUE`: The numeric value you want to convert.
- `from`: Keyword to specify the source unit.
- `<UNIT>`: The source unit you want to convert from.
- `to`: Keyword to specify the target unit.
- `<UNIT>`: The target unit you want to convert to.

## Options

- `-r, --round=PLACES`: Round the result to the specified number of decimal
  places.
- `-h, --help`: Display the help message and exit.
- `-v, --version`: Display version information and exit.

## Examples

1. Convert 100 degrees Celsius to Fahrenheit, rounding to 2 decimal places:

   ```bash
   unicon -r 2 100 from celsius to fahrenheit
   ```

2. Convert 5 kilometers to miles, rounding to 3 decimal places:

   ```bash
   unicon -r 3 5 from kilometers to miles
   ```

3. Display the help message:

   ```bash
   unicon -h
   ```

4. Display version:

   ```bash
   unicon -v
   ```
