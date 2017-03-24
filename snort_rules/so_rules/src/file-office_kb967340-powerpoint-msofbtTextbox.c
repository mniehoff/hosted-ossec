/* XXX DOES NOT USE BUILT-IN DETECTION FUNCTION XXX
 * alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Microsoft Office PowerPoint malformed msofbtTextbox exploit attempt"; flow:to_client,established; flowbits:isset,file.ppt; content:"|0d f0|"; metadata:service http, policy security-ips drop, policy balanced-ips drop; reference:url,technet.microsoft.com/en-us/security/bulletin/MS09-017; reference:cve,2009-0556; classtype:attempted-user; sid:15454;)
 */
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_WRAP(code) code
#else
#define DEBUG_WRAP(code)
#endif


/* declare detection functions */
int rule15454eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule15454flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule15454option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule15454flow0
    }
};
/* flowbits:isset "file.ppt"; */
static FlowBitsInfo rule15454flowbits1 =
{
    "file.ppt",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule15454option1 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule15454flowbits1
    }
};
// content:"|0D F0|", depth 0; 
static ContentInfo rule15454content2 = 
{
    (uint8_t *) "|0D F0|", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED|CONTENT_RELATIVE, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule15454option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule15454content2
    }
};

/* references for sid 15454 */
/* reference: cve "2009-0556"; */
static RuleReference rule15454ref1 = 
{
    "cve", /* type */
    "2009-0556" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS09-017"; */
static RuleReference rule15454ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS09-017" /* value */
};

static RuleReference *rule15454refs[] =
{
    &rule15454ref1,
    &rule15454ref2,
    NULL
};
/* metadata for sid 15454 */
/* metadata:service http, policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule15454service1 = 
{
    "service http"
};


//static RuleMetaData rule15454policy1 = 
//{
//    "policy balanced-ips drop"
//};
//
//static RuleMetaData rule15454policy2 = 
//{
//    "policy security-ips drop"
//};

static RuleMetaData rule15454policy3 = 
{
    "policy max-detect-ips drop"
};

static RuleMetaData *rule15454metadata[] =
{
    &rule15454service1,
//    &rule15454policy1,
//    &rule15454policy2,
    &rule15454policy3,
    NULL
};

RuleOption *rule15454options[] =
{
    &rule15454option0,
    &rule15454option1,
    &rule15454option2,
    NULL
};

Rule rule15454 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
   
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       15454, /* sigid */
       10, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "FILE-OFFICE Microsoft Office PowerPoint malformed msofbtTextbox exploit attempt",     /* message */
       rule15454refs /* ptr to references */
       ,rule15454metadata
   },
   rule15454options, /* ptr to rule options */
   &rule15454eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule15454eval(void *p) {
    const uint8_t *cursor_normal, *end_of_payload, *cursor_detect;
    SFSnortPacket *sp = (SFSnortPacket *) p;
    const uint8_t *tmp_ptr;

    uint32_t msofbtTextboxSize, recLen;

    const uint8_t *msofbtTextbox_end;

    int foundTextHeaderAtom, foundOutlineTextRefAtom;

    DEBUG_WRAP(char rulename[] = "rule15454eval");
    DEBUG_WRAP(printf("%s: enter\n", rulename));

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;

   // flow:established, to_client;
   if(checkFlow(p, rule15454options[0]->option_u.flowFlags) <= 0 )
      return RULE_NOMATCH;

   if(processFlowbits(p, rule15454options[1]->option_u.flowBit) <= 0)
      return RULE_NOMATCH;

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &cursor_normal, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   DEBUG_WRAP(printf("%s: searching content\n", rulename));

   while(contentMatch(p, rule15454options[2]->option_u.content, &cursor_normal) > 0) {
      DEBUG_WRAP(printf("%s: found 0xFOOD\n", rulename));

      if(cursor_normal + 16 >= end_of_payload)
         return RULE_NOMATCH;

      cursor_detect = cursor_normal;

      msofbtTextboxSize = *cursor_detect++;
      msofbtTextboxSize |= *cursor_detect++ << 8;
      msofbtTextboxSize |= *cursor_detect++ << 16;
      msofbtTextboxSize |= *cursor_detect++ << 24;
      DEBUG_WRAP(printf("msofbtTextboxSize=0x%08x\n", msofbtTextboxSize));

      msofbtTextbox_end = cursor_detect + msofbtTextboxSize;

      if(msofbtTextbox_end > end_of_payload || msofbtTextbox_end < cursor_detect)
         msofbtTextbox_end = end_of_payload;

      foundTextHeaderAtom = 0;
      foundOutlineTextRefAtom = 0;

      while(cursor_detect + 4 < msofbtTextbox_end) {
         cursor_detect += 2;  // skip recVer and recInstance
         DEBUG_WRAP(printf("AtomType: 0x%02x%02x\n", *(cursor_detect + 1), *cursor_detect));
         if(*(cursor_detect + 1) == 0x0f) {
            if(*cursor_detect == 0x9f) {
               DEBUG_WRAP(printf("found TextHeaderAtom\n"));
               foundTextHeaderAtom = 1;
            } else if(*cursor_detect == 0x9e) {
               DEBUG_WRAP(printf("found OutlineTextRefAtom\n"));
               foundOutlineTextRefAtom = 1;
            }

            if(foundTextHeaderAtom && foundOutlineTextRefAtom)
               return RULE_MATCH;
         }

         if(cursor_detect + 6 >= msofbtTextbox_end)
            break;  // Try to find next tag //return RULE_NOMATCH;

         cursor_detect += 2; // skip rectType

         recLen = *cursor_detect++;
         recLen |= *cursor_detect++ << 8;
         recLen |= *cursor_detect++ << 16;
         recLen |= *cursor_detect++ << 24;
         DEBUG_WRAP(printf("recLen=0x%08x\n", recLen));

         tmp_ptr = cursor_detect + recLen;

         // int overflow check
         if(tmp_ptr < cursor_detect)
            break; // Try to find next tag //return RULE_NOMATCH;

         cursor_detect = tmp_ptr;
      }
   }

   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule15454,
    NULL
};
*/

