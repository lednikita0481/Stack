const unsigned long long STACK_LEFT_CANNERY_VALUE = 0x8BADBEEF;
const unsigned long long STACK_RIGHT_CANNERY_VALUE = 0x8BADF00D;
const unsigned long long DATA_LEFT_CANNERY_VALUE =  0x32DED300;
const unsigned long long DATA_RIGHT_CANNERY_VALUE =  0x64DED600;
const double double_poison = 0xBADD00B1E;
const int integer_poison = 0xCCCCCCCC;

int Error = 0;

struct creation_info
{
    const char* file;
    const char* function;
    int line;
    const char* first_name;
};

struct Stack
{
    unsigned long long Stack_Left_Cannery;
    unsigned long long* Data_Left_Cannery;
    Elem_t* data;
    unsigned long long* Data_Right_Cannery;
    size_t size;
    size_t capacity;
    creation_info info;
    #if (defined(NDEBUG))
        size_t data_hash;\
        size_t stack_hash;
    #endif
    unsigned long long Stack_Right_Cannery;
};

enum Stack_Errors
{
    No_Errors          = 0,
    Null_Ptr_Err       = 1 << 0,
    Pop_Impossible_Err = 1 << 1,
    Null_Capacity_Err  = 1 << 2,
    Size_More_Cap_Err  = 1 << 3,
    Data_Null_Ptr_Err  = 1 << 4,
    St_Left_Can_Err    = 1 << 5,
    St_Right_Can_Err   = 1 << 6,
    Data_Left_Can_Err  = 1 << 7,
    #if (defined(NDEBUG))
        Data_Hash_Err  = 1 << 9,
        St_Hash_err    = 1 << 10,
    #endif
    Data_Right_Can_Err = 1 << 8
};

void StackConstructor(Stack* stk, size_t capacity);
void StackDestructor(Stack* stk);
void StackPush(Stack* stk, Elem_t value);
Elem_t StackPop(Stack* stk, int* err);
void StackRealloc(Stack* stk);
void* recalloc(void* mass, size_t num, size_t elem_size, size_t elements_now);
int StackErrors(Stack* stk, bool Called_From_Pop);
void StackErrorsDecoder(FILE* stream, int error);
void StackDuump(Stack* stk, int error, const char* func, const char* file, const int line);
size_t Calculate_Hash(void* structure, size_t size_bites);
#if (defined(NDEBUG))
    void Stack_Calculate_Hash(Stack* stk);
#endif

int ReturnPoison(int)
{
    return integer_poison;
}

int ReturnPoison(size_t)
{
    return integer_poison;
}

double ReturnPoison(double)
{
    return double_poison;
}

char ReturnPoison(char)
{
    return 'z' + 1;
}

char* ReturnPoison(char*)
{
    return NULL;
}

unsigned long long ReturnPoison(unsigned long long)
{
    return integer_poison;
}


#define LINES "----------------------------------------------------------------------------------------"

#define StackCtor( a, b ) (a)->info.file = __FILE__;\
    (a)->info.function = __FUNCTION__; (a)->info.line = __LINE__;\
    (a)->info.first_name = #a; StackConstructor( (a), (b) );

#if (!defined(NDEBUG))
    #define ASSERT_OK(stk, a)
#else
    #define ASSERT_OK(stk, a)\
        Error = StackErrors((stk), (a));\
        StackDuump(stk, Error, __FUNCTION__, __FILE__, __LINE__);
#endif



void StackConstructor(Stack* stk, size_t capacity)
{

    stk->Stack_Left_Cannery = STACK_LEFT_CANNERY_VALUE;

    stk->capacity = capacity;

    char* tmp = (char*)calloc((capacity*sizeof(Elem_t) + 2*sizeof(unsigned long long)), sizeof(char));

    stk->Data_Left_Cannery = (unsigned long long*)tmp;
    *(stk->Data_Left_Cannery) = DATA_LEFT_CANNERY_VALUE;

    stk->data = (Elem_t*)(tmp + sizeof(unsigned long long));

    stk->Data_Right_Cannery = (unsigned long long*)((char*)stk->data + sizeof(Elem_t)*(stk->capacity));
    *(stk->Data_Right_Cannery) = DATA_RIGHT_CANNERY_VALUE;


    stk->size = 0;

    stk->Stack_Right_Cannery = STACK_RIGHT_CANNERY_VALUE;

    #if (defined(NDEBUG))
        Stack_Calculate_Hash(stk);
    #endif

    ASSERT_OK(stk, false);
}

void StackDestructor(Stack* stk)
{
    ASSERT_OK(stk, false);

    *(stk->Data_Left_Cannery) = ReturnPoison(DATA_LEFT_CANNERY_VALUE);
    *(stk->Data_Right_Cannery) = ReturnPoison(DATA_RIGHT_CANNERY_VALUE);

    stk->Data_Left_Cannery = NULL;
    stk->Data_Right_Cannery = NULL;
    stk->Stack_Left_Cannery = ReturnPoison(stk->Stack_Left_Cannery);
    stk->Stack_Right_Cannery = ReturnPoison(stk->Stack_Right_Cannery);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = ReturnPoison(stk->data[i]);
    }

    *(stk->Data_Left_Cannery) = ReturnPoison(*(stk->Data_Left_Cannery));
    *(stk->Data_Right_Cannery) = ReturnPoison(*(stk->Data_Right_Cannery));
    free(stk->data);
    free(stk->Data_Left_Cannery);
    free(stk->Data_Right_Cannery);

    stk->capacity = ReturnPoison(stk->capacity);
    stk->size = ReturnPoison(stk->size);
    stk->data = NULL;
    stk->Data_Left_Cannery = NULL;
    stk->Data_Right_Cannery = NULL;
    stk->Stack_Left_Cannery = ReturnPoison(stk->Stack_Left_Cannery);
    stk->Stack_Right_Cannery = ReturnPoison(stk->Stack_Right_Cannery);
}

void StackPush(Stack* stk, Elem_t value)
{
    ASSERT_OK(stk, false);

    if (stk->size >= stk->capacity)
    {
        StackRealloc(stk);
    }

    stk->data[stk->size++] = value;

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif
    

    ASSERT_OK(stk, false);
}

Elem_t StackPop(Stack *stk, int* err = NULL)
{
    ASSERT_OK(stk, true);

    if (stk->size <= 0)
    {
        if (err != NULL) *err = StackErrors(stk, false);
        Elem_t shit;
        return ReturnPoison(shit);
    }

    Elem_t element = stk->data[--stk->size];
    stk->data[stk->size + 1] = ReturnPoison(stk->data[stk->size]);

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif

    if ((stk->size)*4 <= stk->capacity)
    {
        StackRealloc(stk);
    }

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif

    if (err != NULL) *err = StackErrors(stk, false);

    ASSERT_OK(stk, false);

    return element;
}

void StackRealloc(Stack* stk)
{
    ASSERT_OK(stk, false);

    char* tmp = NULL;
    if (stk->size >= stk->capacity)
    {
        tmp = (char*)recalloc(stk->Data_Left_Cannery, (2*sizeof(unsigned long long) + sizeof(Elem_t)*(stk->capacity)*2),
                                    sizeof(char), (sizeof(unsigned long long) + (sizeof(Elem_t)*(stk->capacity))));
        stk->capacity *= 2;
    }

    else
    {
        tmp = (char*)recalloc(stk->Data_Left_Cannery, (2*sizeof(unsigned long long) + sizeof(Elem_t)*(stk->capacity)/2),
                                    sizeof(char), (2*sizeof(unsigned long long) + (sizeof(Elem_t)*(stk->capacity))));
        stk->capacity /= 2;
    }

    stk->Data_Left_Cannery = (unsigned long long*)tmp;
    stk->data = (Elem_t*)(tmp + sizeof(unsigned long long));
    stk->Data_Right_Cannery = (unsigned long long*)((char*)stk->data + (sizeof(Elem_t)*(stk->capacity)));
    *(stk->Data_Right_Cannery) = DATA_RIGHT_CANNERY_VALUE;

    #if (defined(NDEBUG)) 
        Stack_Calculate_Hash(stk);
    #endif

    ASSERT_OK(stk, false);
}

void* recalloc(void* mass, size_t num, size_t elem_size, size_t elements_now)
{
    char* Mass = (char*)mass;

    size_t bites_to_realloc = num*elem_size;

    if (num < elements_now)
    {
        for (size_t i = num; i < elements_now; i++)
        {
            Mass[i] = ReturnPoison(Mass[i]);
        }
    }

    Mass = (char*)realloc(Mass, bites_to_realloc);

    if (num > elements_now)
    {
        for (size_t reset = elem_size*elements_now; reset < bites_to_realloc; reset++)
        {
            Mass[reset] = '\0';
        }
    }

    return Mass;
}


int StackErrors(Stack* stk, bool Called_From_Pop)
{
    int err_code = No_Errors;

    if(stk == NULL)
    {
        err_code += Null_Ptr_Err;
        return err_code;
    }

    if (Called_From_Pop && stk->size == 0)
    {
        err_code += Pop_Impossible_Err;
    }

    if (stk->capacity == 0)
    {
        err_code += Null_Capacity_Err;
    }

    if (stk->capacity < stk->size)
    {
        err_code += Size_More_Cap_Err;
    }

    if (stk->data == NULL)
    {
        err_code += Data_Null_Ptr_Err;
    }

    if (*(stk->Data_Left_Cannery) != DATA_LEFT_CANNERY_VALUE)
    {
        err_code += Data_Left_Can_Err;
    }

    if (*(stk->Data_Right_Cannery) != DATA_RIGHT_CANNERY_VALUE)
    {
        err_code += Data_Right_Can_Err;
    }

    if (stk->Stack_Left_Cannery != STACK_LEFT_CANNERY_VALUE)
    {
        err_code += St_Left_Can_Err;
    }

    if (stk->Stack_Right_Cannery != STACK_RIGHT_CANNERY_VALUE)
    {
        err_code += St_Right_Can_Err;
    }

    #if (defined(NDEBUG)) 
        size_t tmp_data_hash = stk->data_hash;\
        size_t tmp_stk_hash = stk->stack_hash;\
        stk->stack_hash = 0;\
        Stack_Calculate_Hash(stk);\
        \
        if (stk->data_hash != tmp_data_hash)\
        {\
            err_code += Data_Hash_Err;\
        }\
        \
        if (stk->stack_hash != tmp_stk_hash)\
        {\
            err_code += St_Hash_err;\
        }
    #endif

    return err_code;
}


void StackErrorsDecoder(FILE* stream, int error)
{
    if (error == 0)
    {
        fprintf(stream, "Ok\n");
    }

    if ((error & Null_Ptr_Err) != 0)
    {
        fprintf(stream, "Bad pointer. Impossible to read data\n");
    }

    if ((error & Pop_Impossible_Err) != 0)
    {
        fprintf(stream, "Impossible to pop smth from stack with 0 size\n");
    }

    if ((error & Null_Capacity_Err) != 0)
    {
        fprintf(stream, "Capacity = 0, stack doesn't make sence\n");
    }

    if ((error & Size_More_Cap_Err) != 0)
    {
        fprintf(stream, "Size is bigger then capacity. Cringe\n");
    }

    if ((error & Data_Null_Ptr_Err) != 0)
    {
        fprintf(stream, "Impossible to read data\n");
    }

    if ((error & Data_Left_Can_Err) != 0)
    {
        fprintf(stream, "Left Cannery for data broken\n");
    }

    if ((error & Data_Right_Can_Err) != 0)
    {
        fprintf(stream, "Right Cannery for data broken\n");
    }

    if ((error & St_Left_Can_Err) != 0)
    {
        fprintf(stream, "Left cannery for stack broken\n");
    }

    if ((error & St_Right_Can_Err) != 0)
    {
        fprintf(stream, "Right cannery for stack broken\n");
    }

    #if (defined(NDEBUG))
        if ((error & Data_Hash_Err) != 0)\
        {\
            fprintf(stream, "!!! Hash of data broken\n");\
        }\
        \
        if ((error & St_Hash_err) != 0)\
        {\
            fprintf(stream, "!!! Hash of stack broken\n");\
        }
    #endif
}

void StackDuump(Stack* stk, int error, const char* func, const char* file, const int line)
{
    char File_name[] = "debug.txt";
    FILE* File = fopen(File_name, "a");

    fprintf(File, LINES);
    fprintf(File, "\n");
    fprintf(File, "%s at %s(%d):\n", func, file, line);
    fprintf(File, "Stack[%p]", stk);

    if (!error)
    {
        fprintf(File, "(ok), made at %s in %s(%d) as %s\n", stk->info.function, stk->info.file, stk->info.line, stk->info.first_name);
        fprintf(File, "{\n");
        fprintf(File, "    size     = %d\n", stk->size);
        fprintf(File, "    capacity = %d\n", stk->capacity);
        fprintf(File, "    data[%p]\n", stk->data);
        fprintf(File, "    {\n");

        for (int i = 0; i < stk->capacity; i++)
        {
            fprintf(File, "        %c[%d] = %lf      %s\n", ((i<stk->size)? '*' : ' '), i, stk->data[i], ((stk->data[i] == ReturnPoison(stk->data[i]))? "(POISON)" : " "));
        }

        fprintf(File, "    }\n");
        fprintf(File, "}\n");
    }

    else
    {
        fprintf(File, "(ERROR) \n");
        if ((error & Null_Ptr_Err) != 0)
        {
            fprintf(File, "Bad pointer, impossible to count data\n");
        }

        else
        {
            StackErrorsDecoder(File, error);
        }
    }

    fclose(File);
}

size_t Calculate_Hash(void* Structure, size_t size_bites)
{
    char* structure = (char*)Structure;
    size_t control_sum = 0xDED9EBE1;
    for (size_t i = 0; i < size_bites; i++)
    {
        control_sum += i * structure[i];
    }

    return (((control_sum << 5) + control_sum) + 1);
}

#if (defined(NDEBUG))
    void Stack_Calculate_Hash(Stack* stk)\
    {\
        stk->stack_hash = 0;\
        stk->data_hash = Calculate_Hash(stk->Data_Left_Cannery, (2*sizeof(unsigned long long) + sizeof(Elem_t)*(stk->capacity)));\
        stk->stack_hash = Calculate_Hash(stk, sizeof(Stack));\
    }
#endif

