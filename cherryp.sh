#!/bin/bash
git rev-list --reverse eb72bf4cbe1f6b27a5a964e2fd83180c140274b2^..c170a545082a1fa074818ed6f017e0a2b09b07f6 | while read rev  
do 
	git cherry-pick $rev || break
done
