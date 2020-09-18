struct Vector3
{
    int x, y, z;
};

int squared_norm_1(Vector3 num)
{
    return num.x * num.x + num.y * num.y + num.z * num.z;
}

int squared_norm_2(const Vector3& num)
{
    return num.x * num.x + num.y * num.y + num.z * num.z;
}

int squared_norm_3(const Vector3* num)
{
    return num->x * num->x + num->y * num->y + num->z * num->z;
}

int main()
{
    Vector3 vec;
    vec.x = 5;
    vec.y = 10;
    vec.z = 9;

    return squared_norm_1(vec);
}