#!/bin/bash
git checkout jferluna
git add -A

echo "Commit message"
read commitMessage

git commit -m "$commitMessage"
git push

git checkout master
git merge jferluna
git push

git checkout jferluna 
