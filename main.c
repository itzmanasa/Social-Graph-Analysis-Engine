/* Design a console-based system that models a social network. 
Each user is represented as a node, and friendships are undirected edges. 
The system allows users to add friendships, view mutual friends, 
and get friend suggestions (friends-of-friends who aren’t already direct friends).
1. Add User Add a new user by name (max user limit, say 50). 
2. Add Friendship Connect two users (undirected edge). 
3. View Friends List all direct friends of a user. 
4. View Mutual Friends Input: two user names Output: list of users who are friends with both 
5. Suggest Friends Input: one user name Output: list of friends-of-friends not already in friend list */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 50
#define NAME_LENGTH 40
#define MAX_FRIENDS 50

typedef struct {
    char name[NAME_LENGTH];
    int friends[MAX_FRIENDS];
    int friend_count;
} User;

User users[MAX_USERS];
int user_count=0;

int find_user_index(const char *name){
    for(int i=0;i<user_count;i++){
        if(strcmp(users[i].name, name)==0){
            return i;
        }
    }
    return -1;
}

void add_user(const char *name){
    if(user_count>=MAX_USERS){
        printf("Max user limit reached.\n");
        return;
    }
    if(find_user_index(name)!=-1){
        printf("User already exists.\n");
        return;
    }
    strcpy(users[user_count].name, name);
    users[user_count].friend_count=0;
    user_count++;
}

void add_friendship(const char *name1, const char *name2){
    int idx1=find_user_index(name1);
    int idx2=find_user_index(name2);
    if(idx1==-1 || idx2==-1){
        printf("One or more users not found :(\n");
        return;
    }
    if(idx1==idx2){
        printf("Cannot add friendship to oneself.\n");
        return;
    }
    if(users[idx1].friend_count>=MAX_FRIENDS || users[idx2].friend_count>=MAX_FRIENDS){
        printf("One or more users have reached max friends limit.\n");
        return;
    }
    for(int i=0;i<users[idx1].friend_count;i++){
        if(users[idx1].friends[i]==idx2){
            printf("Friendship already exists.\n");
            return;
        }
    }
    users[idx1].friends[users[idx1].friend_count++]=idx2;
    users[idx2].friends[users[idx2].friend_count++]=idx1;
}

void view_friends(const char *name){
    int idx=find_user_index(name);
    if(idx==-1){
        printf("User not found :(\n");
        return;
    }
    if(users[idx].friend_count==0){
        printf("%s has no friends :_)\n", name);
        return;
    }
    printf("Friends of %s:\n", name);
    for(int i=0;i<users[idx].friend_count;i++){
        printf(" * %s\n", users[users[idx].friends[i]].name);
    }
}

void view_mutual_friends(const char *name1, const char *name2){
    int idx1=find_user_index(name1);
    int idx2=find_user_index(name2);
    if(idx1==-1 || idx2==-1){
        printf("One or more users not found :(\n");
        return;
    }
    printf("Mutual friends of %s and %s:\n", name1, name2);
    for(int i=0;i<users[idx1].friend_count;i++){
        int friend_idx=users[idx1].friends[i];
        for(int j=0;j<users[idx2].friend_count;j++){
            if(friend_idx==users[idx2].friends[j]){
                printf(" * %s\n", users[friend_idx].name);
            }
        }
    }
}

void suggest_friends(const char *name){
    int idx=find_user_index(name);
    if(idx==-1){
        printf("User not found :(\n");
        return;
    }
    int suggested[MAX_USERS]={0};
    int visited[MAX_USERS]={0};  // visited array to track processed users
    visited[idx] = 1;  // mark the current user as visited
    for(int i=0;i<users[idx].friend_count;i++){
        int friend_idx=users[idx].friends[i];
        visited[friend_idx] = 1; // mark direct friends as visited
        for(int j=0;j<users[friend_idx].friend_count;j++){
            int fof_idx=users[friend_idx].friends[j];
            if(!visited[fof_idx]){  // only process if not visited yet
                if(fof_idx!=idx){
                    int is_direct_friend=0;
                    for(int k=0;k<users[idx].friend_count;k++){
                        if(users[idx].friends[k]==fof_idx){
                            is_direct_friend=1;
                            break;
                        }
                    }
                    if(!is_direct_friend){
                        suggested[fof_idx]=1;
                    }
                }
                visited[fof_idx] = 1; // mark this friend of friend as visited
            }
        }
    }
    printf("Friend suggestions for %s:\n", name);
    int found = 0;
    for(int i=0;i<user_count;i++){
        if(suggested[i]){
            printf(" * %s\n", users[i].name);
            found = 1;
        }
    }
    if(!found) printf("(no suggestions)\n");
}

int main(){
    int choice;
    char name1[NAME_LENGTH], name2[NAME_LENGTH];
    while(1){
        printf("\n Social Networking Menu:\n");
        printf("1. Add User\n");
        printf("2. Add Friendship (2 inputs)\n");
        printf("3. View Friends\n");
        printf("4. View Mutual Friends (2 inputs)\n");
        printf("5. Suggest Friends\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                printf("Enter user name: ");
                scanf("%s", name1);
                add_user(name1);
                break;
            case 2:
                printf("Enter first user name: ");
                scanf("%s", name1);
                printf("Enter second user name: ");
                scanf("%s", name2);
                add_friendship(name1, name2);
                break;
            case 3:
                printf("Enter user name: ");
                scanf("%s", name1);
                view_friends(name1);
                break;
            case 4:
                printf("Enter first user name: ");
                scanf("%s", name1);
                printf("Enter second user name: ");
                scanf("%s", name2);
                view_mutual_friends(name1, name2);
                break;
            case 5:
                printf("Enter user name: ");
                scanf("%s", name1);
                suggest_friends(name1);
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}