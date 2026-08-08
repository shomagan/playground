/*Given a string s, return the longest in s.
Example 1:
Input: s = "babad"
Output: "bab"

Explanation: "aba" is also a valid answer.

Example 2:
Input: s = "cbbd"
Output: "bb"

Constraints:
    1 <= s.length <= 1000
    s consist of only digits and English letters.
*/
pub fn longest_palindrome(s: String) -> String {
   let n = s.len();
   if n == 0 {
      return String::new();
   }

   let s_bytes = s.as_bytes();
   let mut start = 0;
   let mut max_len = 1;

   for i in 0..n {
      // Odd length palindromes
      let mut left = i as isize;
      let mut right = i as isize;
      while left >= 0 && right < n as isize && s_bytes[left as usize] == s_bytes[right as usize] {
         let current_len = (right - left + 1) as usize;
         if current_len > max_len {
            max_len = current_len;
            start = left as usize;
         }
         left -= 1;
         right += 1;
      }

      // Even length palindromes
      left = i as isize;
      right = (i + 1) as isize;
      while left >= 0 && right < n as isize && s_bytes[left as usize] == s_bytes[right as usize] {
         let current_len = (right - left + 1) as usize;
         if current_len > max_len {
            max_len = current_len;
            start = left as usize;
         }
         left -= 1;
         right += 1;
      }
   }

   s[start..start + max_len].to_string()
}
