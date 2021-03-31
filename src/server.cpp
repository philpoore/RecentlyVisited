#include "server.h"

const std::string NAME = "Recently Visited";
const std::string VERSION = "0.0.1";
const std::size_t NUM_WORKERS = 10;
const int PORT = 8091;

RecentlyVisited rv;
ServerSocket *server = nullptr;
WorkerPool<Args> *wp;

void welcome_message() { std::cout << NAME << " " << VERSION << "\n"; }

uint64_t now_ms() {
    return std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::milliseconds(1);
}

void handle_client(Args args) {
    int client_fd = args.client_fd;
    struct sockaddr client_addr = args.client_addr;
    std::cout << "new connection\n";
    char buff[1024];

    std::string input;
    std::string cmd;
    std::string user_id;
    std::string okay = "ok\n";

    auto parse_arg = [&]() -> std::string {
        auto it = find(input.begin(), input.end(), ' ');
        if (it == input.end())
            return input;
        auto arg = std::string(input.begin(), it);
        input = input.substr(it - input.begin() + 1);
        return arg;
    };

    while (client_fd != -1) {
        int num_read = read(client_fd, buff, sizeof(buff));

        if (num_read < 0) {
            perror("client error");
            break;
        }

        if (num_read == 0) {
            break;
        }

        // Trim newline and carrage return from end of input.
        while (buff[num_read - 1] == '\n' || buff[num_read - 1] == '\r') {
            num_read--;
        }

        input = std::string(buff, buff + num_read);

        // Extract command from front of input.
        cmd = parse_arg();
        // std::cout << "cmd: " << cmd << "\n";
        if (cmd == "quit") {
            break;
        }

        // Extract user_id
        user_id = parse_arg();

        if (cmd == "set") {
            uint64_t item_id = stoll(parse_arg());

            // std::cout << "user_id: " << user_id << "\n";
            // std::cout << "item_id: " << item_id << "\n";
            rv.data[user_id].set(item_id, now_ms());

            // Reply okay.
            auto num_read = write(client_fd, okay.c_str(), okay.size());
            continue;
        } else if (cmd == "get") {
            std::vector<ll> items;
            if (rv.data.count(user_id)) {
                items = rv.data[user_id].keys();
            }

            std::string output;
            for (auto item : items) {
                output.append(std::to_string(item) + '\n');
            }

            auto num_read = write(client_fd, output.c_str(), output.size());
            continue;
        }
    }

    std::cout << "closing connection\n";
    close(client_fd);
}

void signal_handler(int s) {
    printf("Caught signal %d\n", s);
    if (server)
        delete server;
    if (wp)
        delete wp;
    exit(1);
}

void init_signal_handlers() {
    // Setup signal handlers to clenaup on exit
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

void init_worker_pool() {
    wp = new WorkerPool<Args>(NUM_WORKERS, &handle_client);
}

void init_server() {
    // Setup server
    server = new ServerSocket();
    server->bind(PORT);
    server->listen([&](int client_fd, struct sockaddr client_addr) {
        wp->add(new Args(client_fd, client_addr));
    });
}

void init() {
    init_signal_handlers();
    init_worker_pool();
    init_server();
}
