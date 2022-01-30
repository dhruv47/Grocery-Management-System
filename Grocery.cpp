#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<iomanip>
using namespace std;
int total;

void emp_read()
{
    ifstream file;
    file.open("employee.csv");
    if (!file.is_open())
    {
        cout<<"Error:"<<endl;
    }
    string name,gender,age,number,time;

    cout<<"Name\tGender\tAge\tnumber\tDate Time"<<endl;
    while (file.good())
    {
        getline(file, name, ',');
        getline(file, gender,',');
        getline(file, age,',');
        getline(file, number,',');
        getline(file, time,'\n');

        if(name != "")
        {
            cout<<name<<"\t";
            cout<<gender<<"\t";
            cout<<age<<"\t";
            cout<<number<<"\t";
            cout<<time<<endl;
        }
    }
    file.close();
    return ;
}

void read()
{
    ifstream file;
    file.open("stock.csv");
    if (!file.is_open())
    {
        cout<<"Error:"<<endl;
    }
    string name;
    string quan;
    string pri;
    cout<<"Name\tQuantity\tPrice"<<endl;
    while (file.good())
    {
        getline(file, name, ',');
        getline(file, quan,',');
        getline(file, pri,'\n');

        if(name != "")
        {
            cout<<setw(10)<<name<<"\t";
            cout<<setw(10)<<quan<<"\t";
            cout<<setw(10)<<pri<<endl;
        }
        
    }
    file.close();
}

void write(string name, int q,int p)
{
    ofstream fout;
    // opens an existing csv file or creates a new file.
    fout.open("stock.csv", ios ::app);
    fout<<name<<",";
    fout<<q<<",";
    fout<<p<<"\n";
    fout.close();
}

void update(string name ,int q, int condition, int p = 0) //condition to distinguish between update or addition. condition =0 when called through additem, condition =1 for billing
{
    string line;
    int var_qua,var_pri ,b=1,a=0;
    ifstream myfile2; //for reading records
    myfile2.open("stock.csv");

    ofstream temp;
    temp.open("temp.csv");

    while (getline(myfile2, line,'\n'))
    {
        // cout<<typeid(line).name()<<endl;
        int pos = line.find(",");
        string curr_name = line.substr(0,pos );
        string sub_line = line.substr(pos+1);
        int pos2 = sub_line.find(",");
        string curr_qua = sub_line.substr(0,pos2);
        string curr_pri = sub_line.substr(pos2+1);
        // cout<<curr_name<<curr_qua<<curr_pri<<endl;


        if (curr_name != name)
        {
            temp << curr_name <<','<<curr_qua<<','<<curr_pri<<'\n';
        }
        else
        {
            a = 1;
            if(q < stoi(curr_qua))
            {
                var_qua = stoi(curr_qua);
                var_pri = stoi(curr_pri);
                total = q*var_pri + total;
            }
            else
            {
                cout<<"The required quantity of item is currently not available in stock. Please enter less quantity."<<endl;
                temp << curr_name <<','<<curr_qua<<','<<curr_pri<<'\n';
                b= 0;
            }
        }
        
    }
    myfile2.close();
    temp.close();
    remove("stock.csv");
    rename("temp.csv", "stock.csv");
    if(condition==0)//for addition of item
    {
        if(a == 0)
        {
            write(name,q,p);
            return ;
        }
        else
        {
        string new_name,gpa;
        //Now add new Record to file
        ofstream myfile;
        myfile.open("stock.csv", ios::app | ios::out);
        myfile<<name<<',';
        myfile<<var_qua + q<<',';
        myfile<<p<<'\n';
        return ;
        }
    }

    if(a==0)
    {
        cout<<"Item currently not available.\n";
        return ;
    }

    if(b==1 && a==1) //for billing purpose items
    {
        string new_name,gpa;
        //Now add new Record to file
        ofstream myfile;
        myfile.open("stock.csv", ios::app | ios::out);
        myfile<<name<<',';
        myfile<<var_qua - q<<',';
        myfile<<var_pri<<'\n';
        return ;
    }
}

void del(string find)
{
    string line;

    ifstream myfile2; //for reading records
   myfile2.open("data.csv");

    ofstream temp;
    temp.open("temp.csv");
    while (getline(myfile2, line,'\n'))
    {
        // cout<<typeid(line).name()<<endl;
        int pos = line.find(",");
        string name = line.substr(0,pos);
        string sub_line = line.substr(pos+1);
        int pos2 = sub_line.find(",");
        string roll = sub_line.substr(0,pos2);
        string city = sub_line.substr(pos2+1);
        cout<<name<<roll<<city<<endl;


        if (name != find)
        {
            temp << name <<','<<roll<<','<<city<<'\n';
        }
        
    }
    myfile2.close();
    temp.close();
    remove("data.csv");
    rename("temp.csv", "data.csv");
}

void add_emp()
{
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    string name, gender;
    int age, number;
    cout<<"Enter Name: ";
    cin>>name;
    cout<<"Enter gender: ";
    cin>>gender;
    cout<<"Enter age :";
    cin>>age;
    cout<<"Enter your contact number: ";
    cin>>number;
    cout<<name<<", your are now officially apointed as an employee for this store!!!!\n";
    ofstream emp;
    // opens an existing csv file or creates a new file.
    emp.open("employee.csv", ios ::app);
    emp<<name<<",";
    emp<<gender<<",";
    emp<<age<<",";
    emp<<number<<",";
    emp<<dt<<"\n";
    emp.close();
    return ;
}

class items{
    protected:
        string iname;
        int q,p;
    public:
        void add_item();
        void read_item();
        void bill_input();
        void del_item();

};

void items:: add_item()
{
    iname = ' ';
    cout<<"Enter the Name, Quantity and Price of items in order:\n";
    while(true)
    {
        cin>>iname;
        if(iname == "ok")
        break;
        cin>>q>>p;
        update(iname,q,0,p);
    }
    cout<<"!!Item added successfully!!";
}

void items::read_item()
{
    read();
    cout<<"-------------------------------\n";
}

void items::bill_input()
{
    total =0;
    cout<<"Enter the name of item and quantity:\n ";
    while(true)
    {
        cin>>iname;
        if(iname == "ok")
        {break;}
        cin>>q;
        update(iname,q,1);
    }
    cout<<"Your total is "<<total<<" Rs.\n";
    cout<<"-------------------------------\n";
}

void items::del_item()
{
    cout<<"Name the item you want to delete: ";
    cin>>iname;
    del(iname);
}

int main()
{   
    int choice,c=1;
    items ob;
    while(c==1)
    {
        cout<<"\t\t:::Grosery Management System:::"<<endl;
        cout<<"\t1> Create Bill \n\t2> Display Stocks\n\t3> Add/Modify/Delete Stocks \n\t4> Employee Details \n\t5> Exit"<<endl;
        cout<<"Enter your choice:";
        cin>>choice;
        switch(choice)
        {
            case 1:
                //show menu and then billing
                system("cls");
                ob.read_item();
                ob.bill_input();
                break;

            case 2:
                ob.read_item();
                break;

            case 3:
                //1.add or update currents stocks
                //2.display current stocks
                //3.remove stocks
                system("cls");
                int choice3;
                cout<<"\t\t:::Stocks:::"<<endl;
                cout<<"\t1> Add Stocks \n\t2> Delete Stocks \n";
                cout<<"Enter your choice of operation:";
                cin>>choice3;
                switch(choice3)
                {
                    case 1:
                        ob.add_item();
                        break;
                    case 2:
                        ob.del_item();
                        break;
                };
                break;
            case 4:
                //1.add employee details 
                //2.display employee details
                system("cls");
                int choice4;
                cout<<"\t\t:::Employee Management:::"<<endl;
                cout<<"\t1> All Employee details \n\t2> Add Employee \n";
                cout<<"\tEnter your choice of operation:";
                cin>>choice4;
                switch(choice4)
                {
                    case 1:
                        emp_read();
                        break;
                    case 2:
                        add_emp();
                        break;
                }; 
                break;
            case 5:
                c=0;
                break;
        }
    }
    cout<<"Have a nice day!!!\n";
    
}