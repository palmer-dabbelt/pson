$PTEST_BINARY --input $INPUT --output $OUTPUT

diff -u $OUTPUT $OUTPUT.gold
