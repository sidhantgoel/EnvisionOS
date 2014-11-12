
void KeMain(void *li);

void ep_kernel(void *li)
{
	fpinit();
	KeMain(li);
}