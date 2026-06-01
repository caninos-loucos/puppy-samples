# puppy-samples

west build --sysbuild -p -b poodle-toy-v1 hello
west build --sysbuild -p -b poodle-toy-v1 songs
west build --sysbuild -p -b poodle-toy-v1 rgb

west flash --domain hello --port /dev/ttyUSB0
west flash --domain songs --port /dev/ttyUSB0
west flash --domain rgb --port /dev/ttyUSB0

