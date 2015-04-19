#ifndef FQ_H
#define FQ_H

#include "generic.h"

struct fq {
    string seq_ID;

    //qname is the name of the read
    string qname;

    //flags is a bit field encoding some yes/no pieces of information about whether and how the read aligned
    int flags;

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

    /*
    extras tab-separated "extra" fields, usually optional and aligner-specific but often very important!
    http://bio-bwa.sourceforge.net/bwa.shtml#4
    */

    //NM i Edit distance to the reference, including ambiguous bases but excluding clipping
    string NM;
    //AS i Alignment score generated by aligner
    string AS;
    //MD Z String for mismatching positions. Regex : [0-9]+(([A-Z]|\^[A-Z]+)[0-9]+)*7
    string MD;
    //XS  Suboptimal alignment score
    string XS;
    //NM:i:0  MD:Z:100        AS:i:100        XS:i:20

    /*
    Extras includes
    X? ? Reserved fields for end users (together with Y? and Z?)
    AM i The smallest template-independent mapping quality of segments in the rest
    AS i Alignment score generated by aligner
    BC Z Barcode sequence, with any quality scores stored in the QT tag.
    BQ Z Offset to base alignment quality (BAQ), of the same length as the read sequence. At the
    i-th read base, BAQi = Qi − (BQi − 64) where Qi is the i-th base quality.
    CC Z Reference name of the next hit; ‘=’ for the same chromosome
    CM i Edit distance between the color sequence and the color reference (see also NM)
    CO Z Free-text comments
    CP i Leftmost coordinate of the next hit
    CQ Z Color read quality on the original strand of the read. Same encoding as QUAL; same
    length as CS.
    CS Z Color read sequence on the original strand of the read. The primer base must be included.
    CT Z Complete read annotation tag, used for consensus annotation dummy features.
    E2 Z The 2nd most likely base calls. Same encoding and same length as QUAL.
    FI i The index of segment in the template.
    FS Z Segment suffix.
    FZ B,S Flow signal intensities on the original strand of the read, stored as (uint16 t)
    round(value * 100.0).
    LB Z Library. Value to be consistent with the header RG-LB tag if @RG is present.
    H0 i Number of perfect hits
    H1 i Number of 1-difference hits (see also NM)
    H2 i Number of 2-difference hits
    HI i Query hit index, indicating the alignment record is the i-th one stored in SAM
    IH i Number of stored alignments in SAM that contains the query in the current record
    MC Z CIGAR string for mate/next segment
    MD Z String for mismatching positions. Regex : [0-9]+(([A-Z]|\^[A-Z]+)[0-9]+)*7
    MQ i Mapping quality of the mate/next segment
    NH i Number of reported alignments that contains the query in the current record
    NM i Edit distance to the reference, including ambiguous bases but excluding clipping
    OQ Z Original base quality (usually before recalibration). Same encoding as QUAL.
    OP i Original mapping position (usually before realignment)
    OC Z Original CIGAR (usually before realignment)
    PG Z Program. Value matches the header PG-ID tag if @PG is present.
    PQ i Phred likelihood of the template, conditional on both the mapping being correct
    PT Z Read annotations for parts of the padded read sequence8
    PU Z Platform unit. Value to be consistent with the header RG-PU tag if @RG is present.
    QT Z Phred quality of the barcode sequence in the BC (or RT) tag. Same encoding as QUAL.
    Q2 Z Phred quality of the mate/next segment sequence in the R2 tag. Same encoding as QUAL.
    R2 Z Sequence of the mate/next segment in the template.
    RG Z Read group. Value matches the header RG-ID tag if @RG is present in the header.
    RT Z Deprecated alternative to BC tag originally used at Sanger.
    SA Z Other canonical alignments in a chimeric alignment, in the format of:
    (rname,pos,strand,CIGAR,mapQ,NM ;)+. Each element in the semi-colon delimited
    list represents a part of the chimeric alignment. Conventionally, at a supplementary
    line, the first element points to the primary line.
    SM i Template-independent mapping quality
    TC i The number of segments in the template.
    U2 Z Phred probility of the 2nd call being wrong conditional on the best being wrong. The
    same encoding as QUAL.
    UQ i Phred likelihood of the segment, conditional on the mapping being correct
    */
};

#endif