#!/bin/bash
git add -A .

git status

read -r -p "Are you sure to push changes? [y/n] " input
case $input in
  [Yy])
    echo 
    read -r -p "Commit message> " line
    echo $line
    git commit -m "$line"
    git push origin master
    ;;
    
  [nN])
    ;;
    
  *)
    echo "Invalid input..."
    ;;
esac
    
