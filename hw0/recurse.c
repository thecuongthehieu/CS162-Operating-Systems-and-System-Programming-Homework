int recur(int i) {
    /* A stack allocated variable within a recursive function */
    int j = i;

    if (i > 0) {
        return recur(i - 1);
    }

    return 0;
}
