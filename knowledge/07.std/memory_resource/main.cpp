//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <vector>

class LoggingResource final : public std::pmr::memory_resource
{
public:
    explicit LoggingResource(
        std::pmr::memory_resource* upstream =
            std::pmr::new_delete_resource(),
        const char* name = "LoggingResource")
        : upstream_(upstream), name_(name)
    {
    }

private:
    void* do_allocate(
        std::size_t bytes,
        std::size_t alignment) override
    {
        void* ptr = upstream_->allocate(bytes, alignment);

        std::cout << "[" << name_ << "] ALLOC"
                  << " | bytes=" << bytes
                  << " | alignment=" << alignment
                  << " | ptr=" << ptr
                  << '\n';

        return ptr;
    }

    void do_deallocate(
        void* ptr,
        std::size_t bytes,
        std::size_t alignment) override
    {
        std::cout << "[" << name_ << "] FREE"
                  << " | bytes=" << bytes
                  << " | alignment=" << alignment
                  << " | ptr=" << ptr
                  << '\n';

        upstream_->deallocate(ptr, bytes, alignment);
    }

    bool do_is_equal(
        const std::pmr::memory_resource& other)
        const noexcept override
    {
        return this == &other;
    }

    std::pmr::memory_resource* upstream_;
    const char* name_;
};

int main()
{
    LoggingResource resource;

    // Test 1: Vector growth
    std::cout << "\n=== Test 1: Vector Growth ===\n";

    {
        std::pmr::vector<int> values(&resource);

        for (int i = 0; i < 5; ++i)
        {
            values.push_back(i * 10);

            std::cout << "[INFO] Inserted value: " << i * 10
                      << " | Size: " << values.size()
                      << " | Capacity: " << values.capacity()
                      << '\n';
        }

        std::cout << "[INFO] Vector is about to be destroyed.\n";
    }

    // Test 2: Reserve memory in advance
    std::cout << "\n=== Test 2: Reserve Capacity ===\n";

    {
        std::pmr::vector<int> values(&resource);

        values.reserve(100);

        std::cout << "[INFO] Capacity after reserve: "
                  << values.capacity() << '\n';

        for (int i = 0; i < 100; ++i)
        {
            values.push_back(i);
        }

        std::cout << "[INFO] Size after insertion: "
                  << values.size() << '\n';

        std::cout << "[INFO] Capacity after insertion: "
                  << values.capacity() << '\n';
    }

    // Test 3: Nested PMR containers
    std::cout << "\n=== Test 3: Nested PMR Containers ===\n";

    {
        std::pmr::vector<std::pmr::vector<int>> matrix(&resource);

        matrix.emplace_back();
        matrix.back().push_back(100);
        matrix.back().push_back(200);

        matrix.emplace_back();
        matrix.back().push_back(300);

        std::cout << "[INFO] matrix[0][0] = "
                  << matrix[0][0] << '\n';

        std::cout << "[INFO] matrix[0][1] = "
                  << matrix[0][1] << '\n';

        std::cout << "[INFO] matrix[1][0] = "
                  << matrix[1][0] << '\n';
    }

    std::cout << "\n=== All Tests Completed ===\n";

    return 0;
}
