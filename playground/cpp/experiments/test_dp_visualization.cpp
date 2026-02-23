#include "max_summ_for_k_seq_pair.hpp"
#include <iostream>
#include <iomanip>

int main() {
    std::vector<int> nums1 = {1, -2, 3, 0, 4, -1, 8};
    std::vector<int> nums2 = {3, 2, 0, -2, 7};
    int k = 3;

    int n = nums1.size();
    int m = nums2.size();

    // Create and fill DP table using the algorithm
    const int NEG_INF = std::numeric_limits<int>::min() / 2;
    std::vector<std::vector<std::vector<int>>> dp(n + 1,
        std::vector<std::vector<int>>(m + 1,
            std::vector<int>(k + 1, NEG_INF)));

    // Base case
    for(int i = 0; i <= n; ++i) {
        for(int j = 0; j <= m; ++j) {
            dp[i][j][0] = 0;
        }
    }

    // Fill DP table
    for(int i = 1; i <= n; ++i) {
        for(int j = 1; j <= m; ++j) {
            for(int c = 0; c <= k; ++c) {
                dp[i][j][c] = std::max(dp[i][j][c], dp[i-1][j][c]);
                dp[i][j][c] = std::max(dp[i][j][c], dp[i][j-1][c]);
                if(c > 0) {
                    int product = nums1[i-1] * nums2[j-1];
                    dp[i][j][c] = std::max(dp[i][j][c], dp[i-1][j-1][c-1] + product);
                }
            }
        }
    }

    // Print the DP table for each value of c
    for(int c = 0; c <= k; ++c) {
        std::cout << "\n===== DP table for c=" << c << " (selecting " << c << " pairs) =====\n";
        std::cout << "      ";
        for(int j = 0; j <= m; ++j) {
            std::cout << "j=" << j << "   ";
        }
        std::cout << "\n";

        for(int i = 0; i <= n; ++i) {
            std::cout << "i=" << i << " ";
            for(int j = 0; j <= m; ++j) {
                if(dp[i][j][c] == NEG_INF) {
                    std::cout << std::setw(5) << "-INF ";
                } else {
                    std::cout << std::setw(5) << dp[i][j][c] << " ";
                }
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n===== Final Answer =====\n";
    std::cout << "Maximum score for k=" << k << " pairs: " << dp[n][m][k] << "\n";

    return 0;
}
