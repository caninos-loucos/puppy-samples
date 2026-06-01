# puppy-samples

This repository contains the following samples for the `poodle-toy-v1` board:

- hello
- rgb
- songs

## Build

To build the hello sample:

```sh
west build --sysbuild -p -b poodle-toy-v1 hello
```

To build the songs sample:

```sh
west build --sysbuild -p -b poodle-toy-v1 songs
```

To build the rgb sample:

```sh
west build --sysbuild -p -b poodle-toy-v1 rgb
```

## Flash

Connect the board and flash the desired sample using `/dev/ttyUSB0`.

**Note:**

1. Execute the flash command for the desired sample.
2. Press the RESET button on the board to start the firmware update.
3. A progress bar will be displayed, indicating the flashing progress.
4. Once flashing is complete, press the RESET button again to start the sample.

To flash the hello sample:

```sh
west flash --domain hello --port /dev/ttyUSB0
```

To flash the songs sample:

```sh
west flash --domain songs --port /dev/ttyUSB0
```

To flash the rgb sample:

```sh
west flash --domain rgb --port /dev/ttyUSB0
```

