#include "_tempdir.bash"

cat >$INPUT <<"EOF"
[
  {
    "type": "string",,,
    "data": "something",,
  },
  {
    "other type": "another string",,
    "data": "something else",,,
  },,,
]
EOF

cat >$OUTPUT.gold <<"EOF"
[
  {
    "type": "string",
    "data": "something"
  },
  {
    "other type": "another string",
    "data": "something else"
  }
]
EOF

#include "_harness.bash"
