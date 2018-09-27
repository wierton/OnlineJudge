#include <stack>
#include <vector>
#include <iostream>
#include <string>

class Solution {
  public:
	int longestValidParentheses(std::string s) {
	  std::vector<std::string::iterator> stack;
	  std::size_t max_len = 0;
	  std::string::iterator start = s.begin();
	  for(auto it = s.begin(); it != s.end(); ++it) {
		if(*it == '(') {
		  stack.push_back(it);
		}

		if(*it == ')') {
		  if(stack.size() == 0) {
			start = it + 1;
		  } else if(*stack.back() == '(') {
			stack.pop_back();
			if(stack.empty()) {
			  max_len = std::max(max_len, static_cast<std::size_t>(it - start) + 1);
			} else {
			  max_len = std::max(max_len, static_cast<std::size_t>(it - stack.back()));
			}
		  }
		}
	  }
	  return max_len;
	}
};


int main() {
  Solution sol;

  std::vector<const char *> input = {
	"()", "(()", ")()())", "()(()", ")()())"
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.longestValidParentheses(*it) << "\n";
  }
  return 0;
}
