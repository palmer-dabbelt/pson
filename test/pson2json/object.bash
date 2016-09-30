#include "_tempdir.bash"

cat >$INPUT <<"EOF"
{
  "type": "string",
  "data": "something"
}
EOF

cp $INPUT $OUTPUT.gold

#include "_harness.bash"
