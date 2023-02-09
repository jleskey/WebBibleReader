cd "$(realpath `dirname "$0"`)"

if [[ "$1" == "" ]]; then
    echo "Error: No commit message provided."
    exit 1
fi

git commit --amend -m "$1"

./deploy.sh