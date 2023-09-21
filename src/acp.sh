#!/bin bash

# add-commit-push

cd ../
git add -A
git commit -m "$1"
git push origin main
cd src/
