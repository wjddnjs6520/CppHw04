#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

//책 클래스
class Book {
public:
    //제목, 작가 저장.
    string title;
    string author;
    //생성자.
    Book(const string& title, const string& author)
        : title(title), author(author) {
    }
};

//책 목록 클래스
class BookManager {
private:
    //책 저장할 벡터
    vector<Book> books;

public:
    //책 등록
    void addBook(const string& title, const string& author) {
        //벡터에 저장.
        books.push_back(Book(title, author));
        cout << "책이 추가되었습니다: " << title << " by " << author << endl;
    }

    //전체 책 목록
    void displayAllBooks() const {
        //책 벡터가 비어있다면
        if (books.empty()) {
            cout << "현재 등록된 책이 없습니다." << endl;
            return;
        }
        //비어있지 않다면
        cout << "현재 도서 목록:" << endl;
        for (size_t i = 0; i < books.size(); i++) {
            //제목, 작가 순으로 출력
            cout << "- " << books[i].title << " by " << books[i].author << endl;
        }
    }

    //책 제목을 기준으로 벡터에 있는 책 데이터 포인터 반환
    Book* getBookByTitle(const string& title) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].title == title) {
                return &books[i];
            }
        }
        return nullptr;
    }

    //책 작가를 기준으로 새로운 벡터를 생성해 반환. 작가는 중복이 가능하므로.
    vector<Book> getBooksByAuthor(const string& author) {
        vector<Book> result;
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].author == author) {
                result.push_back(books[i]);
            }
        }
        return result;
    }

    //모든 책 데이터가 담긴 벡터 반환
    vector<Book> getAllBooks() const {
        return books;
    }
};

//책 대여 클래스
class BorrowManager {
private:
    //재고 관리를 위한 unordered_map 생성.
    unordered_map<string, int> stock;

public:
    //재고 초기화. 모든 책을 stock에 등록
    void initializeStock(BookManager& bm) {
        for (const auto& book : bm.getAllBooks()) {
            addStock(book.title);
        }
    }

    //재고 등록. stock에 실제 등록. 초기 권수는 3권
    void addStock(const string& title, int quantity = 3) {
        stock[title] = quantity;
    }

    //재고 목록.
    void displayStock() {
        if (stock.empty()) {
            cout << "등록된 재고가 없습니다." << endl;
            return;
        }
        for (auto it = stock.begin(); it != stock.end(); ++it) {
            cout << "책 제목: " << it->first << ", 남은 권 수: " << it->second << endl;
        }
    }

    //책 제목을 기준으로 대여
    void borrowByTitle(const string& title) {
        //stock에 있고. 재고가 있다면 빌릴 수 있음.
        if (stock.count(title) > 0 && stock.at(title) > 0) {
            stock[title]--;
            cout << title << " 제목의 책을 대여 했습니다. 남은 권수: " << stock[title] << endl;
        }
        else {
            cout << title << " 책이 없거나 재고가 없습니다." << endl;
        }
    }

    void borrowByAuthor(BookManager& bm, const string& author) {
        vector<Book> booksByAuthor = bm.getBooksByAuthor(author);

        if (booksByAuthor.empty()) {
            cout << author << " 작가의 책은 등록되어 있지 않습니다." << endl;
            return;
        }

        // 만약 책이 1권뿐이라면 바로 대여
        if (booksByAuthor.size() == 1) {
            cout << author << " 작가의 책 " << booksByAuthor[0].title << "을(를) 대여합니다." << endl;
            borrowByTitle(booksByAuthor[0].title);
            return;
        }

        // 여러 권 있을 때는 선택
        cout << author << " 작가의 책 목록:" << endl;
        for (size_t i = 0; i < booksByAuthor.size(); ++i) {
            cout << i + 1 << ". " << booksByAuthor[i].title
                << " (남은 권수: " << stock[booksByAuthor[i].title] << ")" << endl;
        }

        cout << "빌릴 책의 번호를 입력하세요: ";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > booksByAuthor.size()) {
            cout << "잘못된 선택입니다." << endl;
            return;
        }

        string selectedTitle = booksByAuthor[choice - 1].title;
        borrowByTitle(selectedTitle);
    }



    void returnBook(const string& title) {
        if (stock.count(title) > 0) {
            stock[title]++;
            cout << title << " 책을 반납했습니다. 현재 남은 권수: " << stock[title] << endl;
        }
        else {
            cout << title << " 책이 재고 목록에 없습니다." << endl;
        }
    }
};

int main() {
    BookManager bm;
    BorrowManager br;
    //책 등록
    bm.addBook("노인과 바다", "헤밍웨이");
    bm.addBook("노인과 바다2", "헤밍웨이");
    bm.addBook("죄와 벌", "도스토예프스키");
    bm.addBook("데미안", "헤르만 헤세");
    cout << endl;

    //책 재고 등록
    br.initializeStock(bm);

    //모든 책 목록
    bm.displayAllBooks();
    cout << endl;

    //재고 목록
    br.displayStock();
    cout << endl;

    //제목 데미안 책 4권 빌리기 실행. 4번째 것은 재고 없어서 불가능해야 함.
    br.borrowByTitle("데미안");
    br.borrowByTitle("데미안");
    br.borrowByTitle("데미안");
    br.borrowByTitle("데미안");
    cout << endl;

    //데미안 책 1권 반납. 재고 1 되어야 함.
    br.returnBook("데미안");
    cout << endl;

    //책 작가이름으로 빌리기. 헤밍웨이가 2권이므로 2권 중 선택.
    br.borrowByAuthor(bm, "헤밍웨이");
    cout << endl;

    br.borrowByAuthor(bm, "헤르만 헤세");
    cout << endl;

    //재고 목록
    br.displayStock();
    cout << endl;
    return 0;
}

