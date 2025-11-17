# A demo of using the C++ SML library with Zephyr

A basic proof of concept using the [boost::sml](https://github.com/boost-ext/sml) C++ library with zephyr.

## Use

### Prereqs
A working knowledge of [Zephyr](https://docs.zephyrproject.org/latest/index.html), at least get through the [getting started](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) and build the `Hello world`.

If you are already setup to develop in Zephyr, then skip on.

### Download, setup and build

This is setup as a [T2 Zephyr workspace](https://docs.zephyrproject.org/latest/develop/west/workspaces.html#t2-star-topology-application-is-the-manifest-repository) application, that is it is all pretty self contained. If you know what this means, you'll know how to hack it if you need to.

If not, once you get through the gatting started the following commands should get you going.

``` console
$ cd ~ && mkdir sml-demo && git clone https://github.com/d-o/z-sml.git
$ python -m venv .venv && . .venv/bin/activate && pip install west
$ cd z-sml && west update
$ west build -p always -b qemu_riscv32
$ west build -t run
```

## Output

```
Light started @ Off
Event "On"	Level 5	State On
Event "Off"	Level 0	State Off
Event "Up"	Level 5	State On
Event "Down"	Level 4	State On
Event "Down"	Level 3	State On
Event "Down"	Level 2	State On
Event "Off"	Level 0	State Off
Event "Up"	Level 2	State On
Event "Up"	Level 3	State On
Event "Up"	Level 4	State On
Event "Up"	Level 5	State On
Event "Up"	Level 5	State On
Event "Down"	Level 4	State On
Event "Down"	Level 3	State On
Event "Down"	Level 2	State On
Event "Down"	Level 1	State On
Event "Down"	Level 0	State Off
Event "Down"	Level 0	State Off
Event "Up"	Level 1	State On
Event "Off"	Level 0	State Off
Event "On"	Level 5	State On
Event "Off"	Level 0	State Off
```

## License

Copyright (c) 2025 Dean Sellers (dean@sellers.id.au) SPDX-License-Identifier: MIT
