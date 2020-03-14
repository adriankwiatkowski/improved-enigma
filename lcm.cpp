#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <future>

long long int gcd(long long int a, long long int b) 
{
	long long int gcd;
	while (true) 
	{
		if (a % b == 0) 
		{
			gcd = b;
			break;
		}
		long long int temp = b;
		b = a % b;
		a = temp;
	}
	return gcd;
}

long long int lcm(long long int a, long long int b) 
{
	return (a / gcd(a, b)) * b;
}

long long int get_lcm(std::vector<long long int> list)
{
	long long int current_lcm = list[0];
	for (int i = 1; i < list.size(); ++i) 
	{
		current_lcm = lcm(current_lcm, list[i]);
	}
	return current_lcm;
}

bool should_check(long long int count, const long long int& start_number, const long long int& number) 
{
	auto is_valid = false;
	std::vector<long long int> temp;
	for (long long int index = 0; count > 0; --count, ++index) 
	{
		temp.push_back(start_number + index);
	}
	if (get_lcm(temp) == number) 
	{
		is_valid = true;
	}
	return is_valid;
}

std::vector<long long int> get_vector_without_duplicates(const std::vector<long long int>& factor_vector) 
{
	auto new_vector = factor_vector;
	std::sort(new_vector.begin(), new_vector.end());
	auto ip = std::unique(new_vector.begin(), new_vector.end());
	new_vector.resize(std::distance(new_vector.begin(), ip));
	return new_vector;
}

std::vector<long long int> get_sorted_factors(const long long int& number) 
{
	std::vector<long long int> result;
	long long int i = 1;

	while (i * i <= number) 
	{
		if (number % i == 0) 
		{
			result.push_back(i);

			if (number / i != i) 
			{
				result.push_back(number / i);
			}
		}
		i++;
	}
	std::sort(result.begin(), result.end());
	return result;
}

void set_list(long long int& count, const long long& start_number, const long long& number, std::vector<long long>& list) 
{
	std::vector<long long int> temp_list;
	long long int index = 0;
	for (; index < 2; ++index, --count) 
	{
		temp_list.push_back(start_number + index);
	}
	do
	{
		if (get_lcm(temp_list) == number)
		{
			list.swap(temp_list);
			break;
		}
		temp_list.push_back(start_number + index);
		++index;
	} while (--count >= 0);
}

std::vector<long long int> do_task(const long long int& number) 
{
	auto sorted_factors = get_sorted_factors(number);
	auto start_number = sorted_factors[0];
	auto last_number = sorted_factors[0];
	long long int count = 1;

	std::vector<long long int> list;

	for (long long int i = 1; i < sorted_factors.size(); i++) 
	{
		auto new_number = sorted_factors[i];
		if (new_number - 1 == last_number) 
		{
			++count;
			last_number = new_number;
		}
		else 
		{
			if (count >= 2) 
			{

				auto is_valid = should_check(count, start_number, number);

				if (is_valid) 
				{
					set_list(count, start_number, number, list);
					if (!list.empty()) 
					{
						break;
					}
				}
			}
			start_number = sorted_factors[i];
			last_number = sorted_factors[i];
			count = 1;
		}
	}

	if (list.empty() && count >= 2)
	{
		set_list(count, start_number, number, list);
	}

	std::vector<long long int> numbers_vector;

	if (!list.empty()) 
	{
		numbers_vector.push_back(list.front());
		numbers_vector.push_back(list.back());
	}
	
	return numbers_vector;
}

void update_map(std::map<long long int, std::vector<long long int>>& numbers_map, const std::vector<long long int>& numbers_vector) 
{

	auto numbers_vector_no_duplicates = get_vector_without_duplicates(numbers_vector);

	std::vector<std::future<std::vector<long long int>>> futures;

	for (auto number : numbers_vector_no_duplicates) 
	{
		futures.push_back(std::async(std::launch::async, do_task, number));
	}

	for (int i = 0; i < futures.size(); ++i) 
	{
		auto task_numbers = futures[i].get();
		numbers_map.insert({ numbers_vector_no_duplicates[i], task_numbers });
	}
}

int main()
{
	std::cout << "Type number for each query" << std::endl;
	std::cout << "Find numeric interval in which lcm of all numbers is equal to given number." << std::endl;
	std::cout << "There must be at least 2 numbers in numeric interval(min a, min b)" << std::endl;

	int number_count;

	std::cout << "Number of numeric intervals: " << std::endl;
	std::cin >> number_count;

	std::vector<long long int> numbers_vector;

	for (int i = 0; i < number_count; ++i) 
	{
		long long int value;

		std::cout << "Type number..." << std::endl;
		std::cin >> value;

		numbers_vector.push_back(value);
	}

	std::map<long long int, std::vector<long long int>> numbers_map;
	update_map(numbers_map, numbers_vector);

	for (int i = 0; i < number_count; ++i) 
	{
		auto task_numbers = numbers_map[numbers_vector[i]];
		if (task_numbers.empty()) 
		{
			std::cout << "Numeric interval doesn't exist" << std::endl;
		}
		else 
		{
			std::cout << task_numbers.front() << " " << task_numbers.back() << std::endl;
		}
	}
}