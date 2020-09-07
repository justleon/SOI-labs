#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "queue.cpp"

void producer(PriQueue &q){
    srand(time(NULL));
    while(true){
        std::cout << "Producent " << q.label << " dodaje element do kolejki." << std::endl;
        std::string msg;
        for(int k = 0; k < 3; k++)
            msg += letters[rand() % 3];
        q.push(std::make_pair(NORM, msg), NORM);
        std::cout << "Dodano wiadomość " << msg << " o priorytecie " << NORM << " do kolejki " << q.label << std::endl;
        std::cout << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void consumerA(PriQueue &q1, PriQueue &q2, PriQueue &q3, int pr){
    srand(time(NULL));
    int clean = 0;
    while(true){
        std::cout << "Konsument " << q1.label << " odbiera wiadomość." << std::endl;
        messageType msg = q1.pop();
        std::cout << "Odebrano " << msg.second << " o priorytecie " << msg.first << std::endl;

        std::string str = msg.second;
        if(msg.first == SPEC)
            clean = 5;

        if(msg.first != 2 && clean == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(clean > 0)
            clean--;

        if(!str.empty()){
            char let = str[0];
            str.erase(0, 1);
            int prob = rand() % 100;
            if(prob < pr)
                str += letters[rand() % 3];

            if(let == 'A'){
                std::cout << "Konsument " << q1.label << " dodaje wiadomość do kolejki " << q1.label << std::endl;
                q1.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q1.label << " dodał wiadomość " << str << " do kolejki " << q1.label << std::endl;
            } else if(let == 'B'){
                std::cout << "Konsument " << q1.label << " dodaje wiadomość do kolejki " << q2.label << std::endl;
                q2.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q1.label << " dodał wiadomość " << str << " do kolejki " << q2.label << std::endl;
            } else if(let == 'C'){
                std::cout << "Konsument " << q1.label << " dodaje wiadomość do kolejki " << q3.label << std::endl;
                q3.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q1.label << " dodał wiadomość " << str << " do kolejki " << q3.label << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void consumerB(PriQueue &q1, PriQueue &q2, PriQueue &q3, int pr){
    srand(time(NULL));
    int clean = 0;
    while(true){
        std::cout << "Konsument " << q2.label << " odbiera wiadomość." << std::endl;
        messageType msg = q2.pop();
        std::cout << "Odebrano " << msg.second << " o priorytecie " << msg.first << std::endl;

        std::string str = msg.second;
        if(msg.first == SPEC)
            clean = 5;

        if(msg.first != 2 && clean == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(clean > 0)
            clean--;

        if(!str.empty()){
            char let = str[0];
            str.erase(0, 1);
            int prob = rand() % 100;
            if(prob < pr)
                str += letters[rand() % 3];

            if(let == 'A'){
                std::cout << "Konsument " << q2.label << " dodaje wiadomość do kolejki " << q1.label << std::endl;
                q1.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q2.label << " dodał wiadomość " << str << " do kolejki " << q1.label << std::endl;
            } else if(let == 'B'){
                std::cout << "Konsument " << q2.label << " dodaje wiadomość do kolejki " << q2.label << std::endl;
                q2.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q2.label << " dodał wiadomość " << str << " do kolejki " << q2.label << std::endl;
            } else if(let == 'C'){
                std::cout << "Konsument " << q2.label << " dodaje wiadomość do kolejki " << q3.label << std::endl;
                q3.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q2.label << " dodał wiadomość " << str << " do kolejki " << q3.label << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void consumerC(PriQueue &q1, PriQueue &q2, PriQueue &q3, int pr){
    srand(time(NULL));
    int clean = 0;
    while(true){
        std::cout << "Konsument " << q3.label << " odbiera wiadomość." << std::endl;
        messageType msg = q3.pop();
        std::cout << "Odebrano " << msg.second << " o priorytecie " << msg.first << std::endl;

        std::string str = msg.second;
        if(msg.first == SPEC)
            clean = 5;

        if(msg.first != 2 && clean == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if(clean > 0)
            clean--;

        if(!str.empty()){
            char let = str[0];
            str.erase(0, 1);
            int prob = rand() % 100;
            if(prob < pr)
                str += letters[rand() % 3];

            if(let == 'A'){
                std::cout << "Konsument " << q3.label << " dodaje wiadomość do kolejki " << q1.label << std::endl;
                q1.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q3.label << " dodał wiadomość " << str << " do kolejki " << q1.label << std::endl;
            } else if(let == 'B'){
                std::cout << "Konsument " << q3.label << " dodaje wiadomość do kolejki " << q2.label << std::endl;
                q2.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q3.label << " dodał wiadomość " << str << " do kolejki " << q2.label << std::endl;
            } else if(let == 'C'){
                std::cout << "Konsument " << q3.label << " dodaje wiadomość do kolejki " << q3.label << std::endl;
                q3.push(std::make_pair(NORM, str), NORM);
                std::cout << "Konsument " << q3.label << " dodał wiadomość " << str << " do kolejki " << q3.label << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void defProducer(PriQueue &q1, PriQueue &q2, PriQueue &q3, int wait_t){
    srand(time(NULL));
    while(true){
        if(q1.size() > q2.size() && q1.size() > q3.size()){
            std::cout << "Producent ochronny dodaje element pusty do " << q1.label << std::endl;
            q1.push(std::make_pair(SPEC, ""), SPEC);
        } else if(q2.size() > q1.size() && q2.size() > q3.size()){
            std::cout << "Producent ochronny dodaje element pusty do " << q2.label << std::endl;
            q2.push(std::make_pair(SPEC, ""), SPEC);
        } else if(q3.size() > q1.size() && q3.size() > q2.size()){
            std::cout << "Producent ochronny dodaje element pusty do " << q3.label << std::endl;
            q3.push(std::make_pair(SPEC, ""), SPEC);
        }
        std::cout << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(wait_t));
    }
}

void specProducer(PriQueue &q1, PriQueue &q2, PriQueue &q3){
    srand(time(NULL));
    while(true){
        int which = rand() % 3;
        if(which == 0){
            std::cout << "Super producent dodaje element do kolejki " << q1.label << std::endl;
            std::string msg;
            for(int k = 0; k < 3; k++)
                msg += letters[rand() % 3];
            q1.push(std::make_pair(HIGH, msg), HIGH);
            std::cout << "Super producent dodał wiadomość " << msg << " o priorytecie " << HIGH << " do kolejki "
                << q1.label << std::endl;
        } else if(which == 1){
            std::cout << "Super producent dodaje element do kolejki " << q2.label << std::endl;
            std::string msg;
            for(int k = 0; k < 3; k++)
                msg += letters[rand() % 3];
            q2.push(std::make_pair(HIGH, msg), HIGH);
            std::cout << "Super producent dodał wiadomość " << msg << " o priorytecie " << HIGH << " do kolejki "
                << q2.label << std::endl;
        } else{
            std::cout << "Super producent dodaje element do kolejki " << q3.label << std::endl;
            std::string msg;
            for(int k = 0; k < 3; k++)
                msg += letters[rand() % 3];
            q3.push(std::make_pair(HIGH, msg), HIGH);
            std::cout << "Super producent dodał wiadomość " << msg << " o priorytecie " << HIGH << " do kolejki "
                << q3.label << std::endl;
        }
        std::cout << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
