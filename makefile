PUSH_COMMENT =

git_push_all : 
	git add .
	git commit -m "normal update(see ChangeLog)"
	git push -u origin master
git_push_all_commit :
	git add .
	git commit -m $(PUSH_COMMENT)
	git push -u origin master