all:
	pebble build

install:        all
	pebble install --phone 192.168.141.100
	pebble logs --phone 192.168.141.100

clean:
	pebble clean
	find . -type f -name "*~" -exec rm -f {} \;