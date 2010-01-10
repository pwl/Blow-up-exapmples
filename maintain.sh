#!/bin/bash

#does not seem to be very usefull, but still
echo "** generating headers **"
for file in $(find . -name "*.c"); do
    header=${file/%c/h};
	
    if [ ! -f $header ]; then
	H=$(echo $header| tr a-z A-Z) # sed -e 's|^\[\^/\]/||g') # -e 's|\.|_|g')
	H=${H##*/}
	H=${H//./_}
	H="_${H}_"
	echo "#ifndef $H" > $header;
	echo "#define $H" >> $header;
  	echo "" >> $header;
  	echo "" >> $header;
	./header_gen.perl $file >> $header;
	echo "" >> $header;
	echo "" >> $header;
	echo "#endif /*$H*/" >> $header;
    fi
done

echo "** updating tag tables **"
# gtags . > /dev/zero
etags $(find . -regex ".*\.\(c\|h\)") > /dev/zero
xref -p yapdes -update > /dev/zero

# needs to be reimplemented, now its a mess
# echo "** creating snippets **"
# ./snippets_generator.sh > /dev/zero
