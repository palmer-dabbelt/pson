set -ex

INPUT="in.pson"
OUTPUT="out.json"

tempdir="$(mktemp -d /tmp/pson-test.XXXXXX)"
trap "rm -rf $tempdir" EXIT
cd "$tempdir"
