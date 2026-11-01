#include <iostream>
#include <string>
#include <vector>

class Comment {
public:
    Comment(const std::string& content, const std::string& author) : content_(content), author_(author) {}
    void Display() const {
        std::cout << author_ << ": " << content_ << std::endl;
    }
private:
    std::string content_;
    std::string author_;
};

class Post {
public:
    Post(const std::string& content, const std::string& author) : content_(content), author_(author), likes_() {}
    const std::string Getauthor() const {
        return author_;
    }
    void AddComment(const std::string& content, const std::string& author) {
        comments_.push_back(Comment(content, author));
    }
    void AddLike(const std::string& author) {
        likes_.push_back(author);
    }
    void Display() const {
        std::cout << author_ << ": " << content_ << std::endl;
        std::cout << "Liked by: ";
        for (const auto& liker : likes_) {
            std::cout << liker << " ";
        }
        std::cout << std::endl;
        std::cout << "Comments: " << std::endl;
        for (const auto& comment : comments_) {
            comment.Display();
        }
    }
private:
    std::string content_;
    std::string author_;
    std:: vector<std::string> likes_;
    std::vector<Comment> comments_;
};

std::vector<Post> all_posts;

class User {
public:
    User(const std::string& name) : name_(name) {}
    void AddPost(const std::string& content) {
        all_posts.push_back(Post(content, name_));
    }
    void RemovePost(int index) {
        all_posts.erase(all_posts.begin() + index);
    }
    void DisplayPosts() const {
        std::cout << "--------------------------------------------------" << std::endl;
        for (const auto& post : all_posts) {
            if (post.Getauthor() == name_) {
                post.Display();
            }
        }
        std::cout << "--------------------------------------------------" << std::endl;
    }
    void AddComment(int post_index, const std::string& content) {
        all_posts[post_index].AddComment(content, name_);
    }
    void AddLike(int post_index) {
        all_posts[post_index].AddLike(name_);
    }
private:
    std::string name_;
};

int main() {
    User user1("Alice");
    User user2("Bob");
    User user3("Charlie");
    user1.AddPost("Hello, world!");
    user2.AddLike(0);
    user2.AddComment(0, "Hi, Alice!");
    user1.AddComment(0, "Hi, Bob!");
    user2.AddPost("Welcome to WeChat, Alice!");
    user1.AddLike(1);
    user3.AddLike(1);
    user1.AddComment(1, "Thanks, Bob!");
    user3.AddComment(1, "Hi, Alice!");
    user1.DisplayPosts();
    user2.DisplayPosts();
    return 0;
}