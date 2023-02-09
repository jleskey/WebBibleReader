cd "$(realpath `dirname "$0"`)"

git push -f
git push -f host

ssh josleskey@cs.mvnu.edu -t "cd ~/Courses/Intro\ to\ Software\ Development/project2/src && git rebase && make"