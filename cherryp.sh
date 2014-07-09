#!/bin/bash
git rev-list --reverse b4fa7bc65d43ec7c30fd54267ab45ebf342be663^..c170a545082a1fa074818ed6f017e0a2b09b07f6 | while read rev  
do 
	git cherry-pick $rev || break
done
