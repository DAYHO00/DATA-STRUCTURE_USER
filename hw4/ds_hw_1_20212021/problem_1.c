#include "env.h"

std::vector<std::vector<std::pair<int, int>>> temp_maps; // 임시로 확인된 쌍을 저장하는 temp map
std::vector<std::pair<int, int>> final_map; // 최종적으로 확인된 연결 쌍을 저장하는 맵
std::vector<std::vector<bool>> checked_pairs; // 이미 확인된 쌍을 추적하는 배열

// A 건물에서 임시로 전선을 연결하는 함수
void link_in_building_A(int u, int v) {
    link_wire(u, v); // A 건물에서 임시로 단자 u와 v를 연결
}

// B 건물에서 임시로 전선을 연결하는 함수
void link_in_building_B(int u, int v) {
    link_wire(u, v); // B 건물에서 임시로 단자 u와 v를 연결
}

// A 건물에서 임시로 연결된 전선을 해제하는 함수
void unlink_in_building_A(int u, int v) {
    unlink_wire(u, v); // A 건물에서 임시로 연결한 전선을 해제
}

// B 건물에서 임시로 연결된 전선을 해제하는 함수
void unlink_in_building_B(int u, int v) {
    unlink_wire(u, v); // B 건물에서 임시로 연결한 전선을 해제
}

// 전류가 흐르는지 체크하고, 흐르면 temp_map에 저장
bool check_and_store(int a, int b) {
    if (checked_pairs[a][b]) return false; // 이미 확인된 쌍은 다시 확인하지 않음
    
    check_connectivity(a, b); // check 호출
    int result;
    std::cin >> result;

    if (result == 1) {
        temp_maps.push_back({{a, b}}); // 전류가 흐르면 temp_map에 저장
        checked_pairs[a][b] = true; // 확인된 쌍을 기록
        return true;
    }
    checked_pairs[a][b] = true; // 연결되지 않았더라도 기록
    return false; // 흐르지 않으면 저장하지 않음
}

// temp_map에서 중복된 pair를 확인하고, 최종적으로 확인된 쌍을 final_map에 저장
void verify_and_store_pairs() {
    std::vector<std::pair<int, int>> verified_pairs;

    // temp_maps에서 중복된 pair 확인
    for (const auto& first_case : temp_maps) {
        for (const auto& second_case : temp_maps) {
            for (const auto& pair_first : first_case) {
                for (const auto& pair_second : second_case) {
                    // 중복된 pair가 있는지 확인하고 저장
                    if (pair_first.first == pair_second.first && pair_first.second == pair_second.second) {
                        verified_pairs.push_back(pair_first);
                    }
                }
            }
        }
    }

    // verified_pairs를 final_map에 저장
    for (auto &pair : verified_pairs) {
        final_map.push_back(pair);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    std::cin >> T; 

    while (T--) {
        int N;
        std::cin >> N;

        final_map.clear(); 
        temp_maps.clear(); /
        checked_pairs.assign(N, std::vector<bool>(N, false)); // 확인된 쌍을 추적하기 위한 배열 초기화

        // A와 B 건물의 연결을 확인
        for (int a = 0; a < N; ++a) {
            for (int b = a + 1; b < N; ++b) {  // a와 b가 동일하지 않도록 수정
                if (checked_pairs[a][b]) continue; // 이미 확인된 쌍은 건너뜀

                // 1. A 건물에서 임시 연결
                link_in_building_A(a, b);

                // 2. B 건물로 이동
                move_to_opposite();

                // 3. B 건물에서 연결 시도
                link_in_building_B(a, b);

                // 4. B 건물에서 check 수행
                if (check_and_store(a, b)) {
                    // 연결 확인되면 바로 unlink하고 다음 작업으로 넘어감
                    unlink_in_building_B(a, b);
                    move_to_opposite();
                    unlink_in_building_A(a, b);

                    continue; // 다음 pair로 넘어가기
                }

                // 연결되지 않으면 바로 다음 작업 진행
                unlink_in_building_B(a, b);
                move_to_opposite();
                unlink_in_building_A(a, b);
            }
        }

        verify_and_store_pairs();

        // 최종적으로 확인된 쌍을 answer 함수에 전달
        answer(final_map);
    }

    return 0;
}