#include <iostream>
#include <thread>
#include <string>
#include <mutex>

class Logger {
public :
	void log(const std::string& message) {
		std::lock_guard<std::mutex> lock(_mutex);//блокировка доступа одного потока пока второй поток имеет доступ.обьявляется в месте где возможна гонка данных
		std::cout << "[" << _messageCounter << "]" << message << "\n";//cout не гарантирует thread safety(безопасный ввывод)
		_messageCounter++;//состояние гонки race condition
	}
private :
	std::mutex _mutex;
	int  _messageCounter = 0;
};

int main() {
	setlocale(LC_ALL, "Russian");
	//Main thread; - разделение на потоки придумали для разделения выполнения различных задач асинхронно
	std::string s;
	std::cin >> s;
	
	Logger logger;
	logger.log("Hello");
	logger.log("info message");

	std::thread t1 = std::thread([&logger]() {
		std::string threadName = "t1";
		for (size_t i = 0; i < 100; i++) {
			logger.log(threadName + "doing" + std::to_string(i) + " iteration of work\n");
		}
		});
	t1.join();
	std::thread t2 = std::thread([&logger]() {
		std::string threadName = "t2";
		for (size_t i = 0; i < 100; i++) {
			logger.log(threadName + "doing" + std::to_string(i) + " iteration of work\n");
		}
		});
	t2.join();
	
	return 0;
}