#include "_tempdir.bash"

cat >$INPUT <<"EOF"
{
  "type": "string",,,
  "data": "something",,
}
EOF

cat >$OUTPUT.gold <<"EOF"
{
  "type": "string",
  "data": "something"
}
EOF

#include "_harness.bash"
