cat $INPUT
$PTEST_BINARY --input $INPUT --output $OUTPUT
cat $OUTPUT

cat $OUTPUT.gold
diff -u $OUTPUT $OUTPUT.gold
