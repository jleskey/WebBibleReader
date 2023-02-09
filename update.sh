cd "$(realpath `dirname "$0"`)"

git add -A

if [[ "$1" == "" ]]; then
    echo "Error: No commit message provided."
    exit 1
fi

git commit -m "$1"

./deploy.sh
