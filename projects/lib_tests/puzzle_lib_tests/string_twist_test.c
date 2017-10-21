#include <stddef.h>
#include <string.h>
#include <pms_common.h>
#include <pms_string_twist.h>

#define MAX_STRLEN 1024

void
apply_string_twists(char* buffer,
                    const char* src, 
                    const size_t* sequence, 
                    size_t sequence_count)
{
    char scratch_pad[MAX_STRLEN];
    strcpy(buffer, src);
    for (size_t i = 0; i < sequence_count; ++i)
    {
        //PMS_INFO("%zu %s", sequence[i] + 1, buffer);
        switch(sequence[i])
        {
            case 0:
                pms_string_twist_1(scratch_pad, buffer);
                break;
            case 1:
                pms_string_twist_2(scratch_pad, buffer);
                break;
            case 2:
                pms_string_twist_3(scratch_pad, buffer);
                break;
            case 3:
                pms_string_twist_4(scratch_pad, buffer);
                break;
            default:
                PMS_WARN("Illegal Sequence");
                break;
        }
        strcpy(buffer, scratch_pad);
    }
}

void
test_string(const char* base_str, const char* target,
            const size_t* sequence, size_t sequence_count,
            size_t* success_count)
{
    char buffer[MAX_STRLEN];
    memset(buffer, 0, MAX_STRLEN);
    apply_string_twists(buffer, base_str, sequence, sequence_count);
    if (strcmp(buffer, target) == 0)
    {
        PMS_INFO("Test passed: Target: %s, Buffer: %s", target, buffer);
    }
    else
    {
        PMS_WARN("Test failed: Target: %s, Buffer: %s", target, buffer);
        (*success_count)--;
    }
}

int
main(PMS_UNUSED int argc, 
     PMS_UNUSED char** argv)
{
    const size_t number_of_tests = 10;
    size_t success_count = number_of_tests;
    
    {
        size_t sequence[] = {0, 3, 0, 3};
        test_string("1234 5678", 
                    "6781 2345", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);
    }

    {
        size_t sequence[] = {2, 0, 3, 2, 2, 2, 0, 2, 2, 0};
        test_string("Simon McCallum", 
                    "monCa imlSMclu", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);
    }

    {
        size_t sequence[] = {3, 2, 0, 1, 3, 0, 3, 2};
        test_string("Multithreaded", 
                    "tithreaeadedMultiMuld edhr", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);
    }

    {
        size_t sequence[] = {3, 0, 0, 0, 1, 2, 0};
        test_string("Game Programming", 
                    "Prmminograg Game", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);  
    }

    {
        size_t sequence[] = {0, 1, 3, 0, 2, 0, 3, 1};
        test_string("Programming Patterns", 
                    "nsrammiProgngPatter ", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);  
    }

    {
        size_t sequence[] = {2, 0, 2, 0, 3, 2, 3, 2};
        test_string("CppCon 2016", 
                    "Cp0 16nopC", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);  
    }

    {
        size_t sequence[] = {1, 2, 2, 0, 0};
        test_string("FTCP Engine Is Best Engine", 
                    " EngineFTCP ngine Es BestI", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);     
    }

    {
        size_t sequence[] = {3, 2, 1, 1, 2, 1, 2, 1, 2, 1};
        test_string("Wish there was proper vim support in Git bash", 
                    " is shroper Wi Gthere wvinpit bash am support", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);     
    }

    {
        size_t sequence[] = {0, 2, 0, 0, 0, 3};
        test_string("Potetstappe", 
                    "aptstpePot aptstpePot", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);     
    }

    {
        size_t sequence[] = {0, 3, 2, 3, 2, 0};
        test_string("Kumocha", 
                    "mochcKumoha Ku", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);     
    }

    {
        size_t sequence[] = {0, 3, 0, 3, 2, 1, 3, 2, 2};
        test_string("This code is best code", 
                    "dcob esstde  iscoeT hi", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);     
    }

    {
        size_t sequence[] = {0, 3, 2, 3, 2, 2, 3, 2};
        test_string("En text", 
                    "E nxte", 
                    sequence, 
                    sizeof(sequence) / sizeof(size_t), 
                    &success_count);     
    }

    return 0;
}
