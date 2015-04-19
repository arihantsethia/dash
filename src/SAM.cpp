#include "headers/SAM.h"
using namespace std;

class SAM
{
public:
	string object_read_file;

	string qname, rname, cigar, qual, rnext;
	int flags, pos, mapq;

	struct fq_point {
		string seq_ID;
		//qname is the name of the read
		string qname;
		//flags is a bit field encoding some yes/no pieces of information about whether and how the read aligned
		string flags;
		//rname is the name of the reference sequence that the read aligned to (if applicable). E.g., might be "chr17" meaning "chromosome 17"
		string rname;
		//pos is the 1-based offset into the reference sequence where the read aligned.
		//http://osdir.com/ml/galaxy-source-control/2012-02/msg00032.html
		int pos;
		//mapq for an aligned read, this is a confidence value; high when we're very confident we've found the correct alignment, low when we''re not confident
		int mapq;
		//cigar indicates where any gaps occur in the alignment
		string cigar;
		//seq read sequence. If read aligned to reference genome's reverse-complement, this is the reverse complement of the read sequence.
		string seq;
		//qual quality sequence. If read aligned to reference genome''s reverse-complement, this is the reverse of the quality sequence.
		string quality_string;
		//extras tab-separated "extra" fields, usually optional and aligner-specific but often very important!
		string extras;
	};
	vector <fq_point> reads;
	SAM(string sam_name)
	{
		object_read_file = sam_name;
		import_file(sam_name);
	}
	~SAM(){
	}

	void import_file(string sam_name)
	{
		ifstream sam_read (cstr(sam_name));
		string str_fq;
		int cnt=0;
		fq_point new_read;
		new_read.flags = match_flags;
		while(getline(sam_read, str_fq))
		{
			cnt%=4;
			if(cnt==0)
			{
				new_read.seq_ID = str_fq;
				new_read.seq_ID[new_read.seq_ID.size()-2]='\0';
			}
			else if(cnt==1)
				new_read.seq = str_fq;
			else if(cnt==3)
				new_read.quality_string = str_fq;
			cnt++;
			if(cnt==4)
				reads.push_back(new_read);
		}
		return;
	}

	string extension_sam(string str)
	{
		string ret;
		for(int i=str.size()-1; i>=0; i--)
		{
			if(str[i]=='.')
			{
				for(int j=0; j<i; j++)
					ret+=str[j];
				break;
			}
		}
		ret+=".sam";
		return ret;
	}

	void create_SAM(SAM obj)
	{
		cout<<"The file to be created is "<<extension_sam(obj.object_read_file)<<endl;
		ofstream sam_read (cstr(extension_sam(obj.object_read_file)));
		sam_read << header;
		sam_read << CO;
		for(int i=0; i<(obj.reads).size(); i++)
		{
			cout<<obj.reads[i].seq_ID<<" "<<i<<obj.reads[i].seq<<" "<<obj.reads[i].quality_string<<endl;
		}
		return;
	}
};

int main(int argc, char *argv[])
{
	string fq_sequence(argv[1]);
	SAM obj(fq_sequence);
	obj.create_SAM(obj);
	return 0;
}