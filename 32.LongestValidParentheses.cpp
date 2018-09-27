#include <stack>
#include <vector>
#include <iostream>
#include <string>

class Solution {
  public:
	int longestValidParentheses(std::string s) {
	  std::stack<char> stack;
	  std::size_t curr_len = 0, max_len = 0;
	  for(auto it = s.begin(); it != s.end(); ++it) {
		if(*it == '(') {
		  stack.push(*it);
		}

		if(*it == ')') {
		  if(stack.size() == 0) {
			curr_len = 0;
		  } else if(stack.top() == '(') {
			stack.pop();
			curr_len ++;
		  }
		}

		if(curr_len >= max_len) {
		  max_len = curr_len;
		}
	  }
	  return max_len * 2;
	}
};


int main() {
  Solution sol;

  std::vector<const char *> input = {
	"(()", ")()())", "()(()"
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.longestValidParentheses(*it) << "\n";
  }
  return 0;
}
