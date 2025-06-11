#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

//å Ŭ����
class Book {
public:
    //����, �۰� ����.
    string title;
    string author;
    //������.
    Book(const string& title, const string& author)
        : title(title), author(author) {
    }
};

//å ��� Ŭ����
class BookManager {
private:
    //å ������ ����
    vector<Book> books;

public:
    //å ���
    void addBook(const string& title, const string& author) {
        //���Ϳ� ����.
        books.push_back(Book(title, author));
        cout << "å�� �߰��Ǿ����ϴ�: " << title << " by " << author << endl;
    }

    //��ü å ���
    void displayAllBooks() const {
        //å ���Ͱ� ����ִٸ�
        if (books.empty()) {
            cout << "���� ��ϵ� å�� �����ϴ�." << endl;
            return;
        }
        //������� �ʴٸ�
        cout << "���� ���� ���:" << endl;
        for (size_t i = 0; i < books.size(); i++) {
            //����, �۰� ������ ���
            cout << "- " << books[i].title << " by " << books[i].author << endl;
        }
    }

    //å ������ �������� ���Ϳ� �ִ� å ������ ������ ��ȯ
    Book* getBookByTitle(const string& title) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].title == title) {
                return &books[i];
            }
        }
        return nullptr;
    }

    //å �۰��� �������� ���ο� ���͸� ������ ��ȯ. �۰��� �ߺ��� �����ϹǷ�.
    vector<Book> getBooksByAuthor(const string& author) {
        vector<Book> result;
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].author == author) {
                result.push_back(books[i]);
            }
        }
        return result;
    }

    //��� å �����Ͱ� ��� ���� ��ȯ
    vector<Book> getAllBooks() const {
        return books;
    }
};

//å �뿩 Ŭ����
class BorrowManager {
private:
    //��� ������ ���� unordered_map ����.
    unordered_map<string, int> stock;

public:
    //��� �ʱ�ȭ. ��� å�� stock�� ���
    void initializeStock(BookManager& bm) {
        for (const auto& book : bm.getAllBooks()) {
            addStock(book.title);
        }
    }

    //��� ���. stock�� ���� ���. �ʱ� �Ǽ��� 3��
    void addStock(const string& title, int quantity = 3) {
        stock[title] = quantity;
    }

    //��� ���.
    void displayStock() {
        if (stock.empty()) {
            cout << "��ϵ� ��� �����ϴ�." << endl;
            return;
        }
        for (auto it = stock.begin(); it != stock.end(); ++it) {
            cout << "å ����: " << it->first << ", ���� �� ��: " << it->second << endl;
        }
    }

    //å ������ �������� �뿩
    void borrowByTitle(const string& title) {
        //stock�� �ְ�. ��� �ִٸ� ���� �� ����.
        if (stock.count(title) > 0 && stock.at(title) > 0) {
            stock[title]--;
            cout << title << " ������ å�� �뿩 �߽��ϴ�. ���� �Ǽ�: " << stock[title] << endl;
        }
        else {
            cout << title << " å�� ���ų� ��� �����ϴ�." << endl;
        }
    }

    void borrowByAuthor(BookManager& bm, const string& author) {
        vector<Book> booksByAuthor = bm.getBooksByAuthor(author);

        if (booksByAuthor.empty()) {
            cout << author << " �۰��� å�� ��ϵǾ� ���� �ʽ��ϴ�." << endl;
            return;
        }

        // ���� å�� 1�ǻ��̶�� �ٷ� �뿩
        if (booksByAuthor.size() == 1) {
            cout << author << " �۰��� å " << booksByAuthor[0].title << "��(��) �뿩�մϴ�." << endl;
            borrowByTitle(booksByAuthor[0].title);
            return;
        }

        // ���� �� ���� ���� ����
        cout << author << " �۰��� å ���:" << endl;
        for (size_t i = 0; i < booksByAuthor.size(); ++i) {
            cout << i + 1 << ". " << booksByAuthor[i].title
                << " (���� �Ǽ�: " << stock[booksByAuthor[i].title] << ")" << endl;
        }

        cout << "���� å�� ��ȣ�� �Է��ϼ���: ";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > booksByAuthor.size()) {
            cout << "�߸��� �����Դϴ�." << endl;
            return;
        }

        string selectedTitle = booksByAuthor[choice - 1].title;
        borrowByTitle(selectedTitle);
    }



    void returnBook(const string& title) {
        if (stock.count(title) > 0) {
            stock[title]++;
            cout << title << " å�� �ݳ��߽��ϴ�. ���� ���� �Ǽ�: " << stock[title] << endl;
        }
        else {
            cout << title << " å�� ��� ��Ͽ� �����ϴ�." << endl;
        }
    }
};

int main() {
    BookManager bm;
    BorrowManager br;
    //å ���
    bm.addBook("���ΰ� �ٴ�", "��ֿ���");
    bm.addBook("���ΰ� �ٴ�2", "��ֿ���");
    bm.addBook("�˿� ��", "�����俹����Ű");
    bm.addBook("���̾�", "�츣�� �켼");
    cout << endl;

    //å ��� ���
    br.initializeStock(bm);

    //��� å ���
    bm.displayAllBooks();
    cout << endl;

    //��� ���
    br.displayStock();
    cout << endl;

    //���� ���̾� å 4�� ������ ����. 4��° ���� ��� ��� �Ұ����ؾ� ��.
    br.borrowByTitle("���̾�");
    br.borrowByTitle("���̾�");
    br.borrowByTitle("���̾�");
    br.borrowByTitle("���̾�");
    cout << endl;

    //���̾� å 1�� �ݳ�. ��� 1 �Ǿ�� ��.
    br.returnBook("���̾�");
    cout << endl;

    //å �۰��̸����� ������. ��ֿ��̰� 2���̹Ƿ� 2�� �� ����.
    br.borrowByAuthor(bm, "��ֿ���");
    cout << endl;

    br.borrowByAuthor(bm, "�츣�� �켼");
    cout << endl;

    //��� ���
    br.displayStock();
    cout << endl;
    return 0;
}

