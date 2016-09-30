#include "_tempdir.bash"

cat >$INPUT <<"EOF"
{
  "type": "array",
  "data": [
    "some data",
    {
      "child type": "string",
      "data": "hello, world"
    }
  ]
}
EOF

cp $INPUT $OUTPUT.gold

#include "_harness.bash"
