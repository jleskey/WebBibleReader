cd "$(realpath `dirname "$0"`)"

git reset --soft HEAD~
git add -A
git commit -C ORIG_HEAD

./deploy.sh