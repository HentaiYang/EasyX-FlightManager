#include "ArraySort.h"

int ArraySort::cmp_lps(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
	return aa->price - bb->price;
}

int ArraySort::cmp_hps(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
    return bb->price - aa->price;
}

int ArraySort::cmp_lbs(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
    if (aa->hour == bb->hour) return aa->minute - bb->minute;
    return aa->hour - bb->hour;
}

int ArraySort::cmp_hbs(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
    if (aa->hour == bb->hour) return bb->minute - aa->minute;
    return bb->hour - aa->hour;
}

int ArraySort::cmp_sts(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
    return aa->during_time - bb->during_time;
}

int ArraySort::cmp_lts(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
    return bb->during_time - aa->during_time;
}

void ArraySort::lowPriceSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_lps);
}

void ArraySort::highPriceSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_hps);
}

void ArraySort::lowBeginSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_lbs);
}

void ArraySort::highBeginSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_hbs);
}

void ArraySort::shortTimeSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_sts);
}

void ArraySort::longTimeSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_lts);
}

void ArraySort::straightSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), cmp_rstt);
}

bool ArraySort::cmp_rlps(Result a, Result b)
{
    uint32 pria = 0;
    uint32 prib = 0;
    for (size_t i = 0; i < a.n; i++)
    {
        pria += (*(a.flt.begin() + i))->price;
    }
    for (size_t i = 0; i < b.n; i++)
    {
        prib += (*(b.flt.begin() + i))->price;
    }
    return pria < prib;
}

bool ArraySort::cmp_rhps(Result a, Result b)
{
    uint32 pria = 0;
    uint32 prib = 0;
    for (size_t i = 0; i < a.n; i++)
    {
        pria += (*(a.flt.begin() + i))->price;
    }
    for (size_t i = 0; i < b.n; i++)
    {
        prib += (*(b.flt.begin() + i))->price;
    }
    return pria > prib;
}

bool ArraySort::cmp_rlbs(Result a, Result b)
{
    uint32 bta_m = 0;
    uint32 btb_m = 0;
    for (size_t i = 0; i < a.n; i++)
    {
        bta_m += (*(a.flt.begin() + i))->hour * 60 + (*(a.flt.begin() + i))->minute;
    }
    for (size_t i = 0; i < b.n; i++)
    {
        btb_m += (*(b.flt.begin() + i))->hour * 60 + (*(b.flt.begin() + i))->minute;
    }
    return bta_m < btb_m;
}

bool ArraySort::cmp_rhbs(Result a, Result b)
{
    uint32 bta_m = 0;
    uint32 btb_m = 0;
    for (size_t i = 0; i < a.n; i++)
    {
        bta_m += (*(a.flt.begin() + i))->hour * 60 + (*(a.flt.begin() + i))->minute;
    }
    for (size_t i = 0; i < b.n; i++)
    {
        btb_m += (*(b.flt.begin() + i))->hour * 60 + (*(b.flt.begin() + i))->minute;
    }
    return bta_m > btb_m;
}

bool ArraySort::cmp_rsts(Result a, Result b)
{
    uint32 ta = 0;
    uint32 tb = 0;
    for (size_t i = 0; i < a.n; i++)
    {
        ta += (*(a.flt.begin() + i))->during_time;
    }
    for (size_t i = 0; i < b.n; i++)
    {
        tb += (*(b.flt.begin() + i))->during_time;
    }
    return ta < tb;
}

bool ArraySort::cmp_rlts(Result a, Result b)
{
    uint32 ta = 0;
    uint32 tb = 0;
    for (size_t i = 0; i < a.n; i++)
    {
        ta += (*(a.flt.begin() + i))->during_time;
    }
    for (size_t i = 0; i < b.n; i++)
    {
        tb += (*(b.flt.begin() + i))->during_time;
    }
    return ta > tb;
}

bool ArraySort::cmp_rstt(Result a, Result b)
{
    return a.n < b.n;
}

int ArraySort::cmp_def(const void* a, const void* b)
{
    Flight* aa = (Flight*)a;
    Flight* bb = (Flight*)b;
    return aa->number - bb->number;
}

void ArraySort::defSort(std::vector<Flight>& data)
{
    qsort(&data[0].number, data.size(), sizeof(data[0]), cmp_def);
}

void ArraySort::lowPriceSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), ArraySort::cmp_rlps);
}

void ArraySort::highPriceSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), ArraySort::cmp_rhps);
}

void ArraySort::lowBeginSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), ArraySort::cmp_rlbs);
}

void ArraySort::highBeginSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), ArraySort::cmp_rhbs);
}

void ArraySort::shortTimeSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), ArraySort::cmp_rsts);
}

void ArraySort::longTimeSort(std::vector<Result>& data)
{
    sort(data.begin(), data.end(), ArraySort::cmp_rlts);
}
